#include <stdafx.h>

#include "file_helpers.h"

#include <qwr/abort_callback.h>
#include <qwr/fb2k_paths.h>
#include <qwr/string_helpers.h>
#include <qwr/text_helpers.h>
#include <qwr/winapi_error_helpers.h>

namespace fs = std::filesystem;

namespace qwr::file
{

std::optional<fs::path> FileDialog(const std::wstring& title,
                                    bool saveFile,
                                    const FileDialogOptions& options)
{
    _COM_SMARTPTR_TYPEDEF(IFileDialog, __uuidof(IFileDialog));
    _COM_SMARTPTR_TYPEDEF(IShellItem, __uuidof(IShellItem));

    try
    {
        IFileDialogPtr pfd;
        HRESULT hr = pfd.CreateInstance((saveFile ? CLSID_FileSaveDialog : CLSID_FileOpenDialog), nullptr, CLSCTX_INPROC_SERVER);
        qwr::error::CheckHR(hr, "CreateInstance");

        DWORD dwFlags;
        hr = pfd->GetOptions(&dwFlags);
        qwr::error::CheckHR(hr, "GetOptions");

        if (options.savePathGuid)
        {
            hr = pfd->SetClientGuid(*options.savePathGuid);
            qwr::error::CheckHR(hr, "SetClientGuid");
        }

        hr = pfd->SetTitle(title.c_str());
        qwr::error::CheckHR(hr, "SetTitle");

        if (!options.filterSpec.empty())
        {
            hr = pfd->SetFileTypes(options.filterSpec.size(), options.filterSpec.data());
            qwr::error::CheckHR(hr, "SetFileTypes");
        }

        //hr = pfd->SetFileTypeIndex(1);
        //qwr::error::CheckHR(hr, "SetFileTypeIndex");

        if (options.defaultExtension.length())
        {
            hr = pfd->SetDefaultExtension(options.defaultExtension.c_str());
            qwr::error::CheckHR(hr, "SetDefaultExtension");
        }

        if (options.defaultFilename.length())
        {
            hr = pfd->SetFileName(options.defaultFilename.c_str());
            qwr::error::CheckHR(hr, "SetFileName");
        }

        IShellItemPtr pFolder;
        hr = SHCreateItemFromParsingName(path::Component().wstring().c_str(), nullptr, IShellItemPtr::GetIID(), reinterpret_cast<void**>(&pFolder));
        qwr::error::CheckHR(hr, "SHCreateItemFromParsingName");

        hr = pfd->SetDefaultFolder(pFolder);
        qwr::error::CheckHR(hr, "SetDefaultFolder");

        hr = pfd->Show(nullptr);
        if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED))
        {
            return std::nullopt;
        }
        qwr::error::CheckHR(hr, "Show");

        IShellItemPtr psiResult;
        hr = pfd->GetResult(&psiResult);
        qwr::error::CheckHR(hr, "GetResult");

        PWSTR pszFilePath = nullptr;
        hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
        qwr::error::CheckHR(hr, "GetDisplayName");

        auto scope = wil::scope_exit([pszFilePath] {
            if (pszFilePath)
            {
                CoTaskMemFree(pszFilePath);
            }
        });

        return pszFilePath;
    }
    catch (const std::filesystem::filesystem_error&)
    {
        // TODO: replace with proper error reporting
        return std::nullopt;
    }
    catch (const QwrException&)
    {
        // TODO: replace with proper error reporting
        return std::nullopt;
    }
}

} // namespace qwr::file
