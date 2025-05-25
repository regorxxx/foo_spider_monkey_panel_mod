#pragma once
#include <shtypes.h>

namespace qwr::file
{

struct FileDialogOptions
{
    std::vector<COMDLG_FILTERSPEC> filterSpec{ { L"All files", L"*.*" } };
    std::wstring defaultExtension = L"";
    std::wstring defaultFilename = L"";
    std::optional<GUID> savePathGuid;
};

std::optional<std::filesystem::path>
FileDialog(const std::wstring& title,
            bool saveFile,
            const FileDialogOptions& options = {});

} // namespace qwr::file
