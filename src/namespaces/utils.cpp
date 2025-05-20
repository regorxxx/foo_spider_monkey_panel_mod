#include <stdafx.h>

#include "utils.h"

#include <config/package_utils.h>
#include <js_engine/js_to_native_invoker.h>
#include <js_objects/fb_metadb_handle.h>
#include <js_objects/gdi_bitmap.h>
#include <js_utils/js_art_helpers.h>
#include <js_utils/js_error_helper.h>
#include <js_utils/js_hwnd_helpers.h>
#include <js_utils/js_object_helper.h>
#include <js_utils/js_property_helper.h>
#include <ui/ui_html.h>
#include <ui/ui_input_box.h>
#include <utils/art_helpers.h>
#include <utils/colour_helpers.h>
#include <utils/custom_sort.h>
#include <utils/edit_text.h>
#include <utils/download_file.h>
#include <utils/gdi_error_helpers.h>

#include <qwr/file_helpers.h>
#include <qwr/winapi_error_helpers.h>

// StringCchCopy, StringCchCopyN
#include <StrSafe.h>
#include <fcntl.h>
#include <io.h>

#include <filesystem>
#include <wil/resource.h>

using namespace smp;

namespace fs = std::filesystem;

namespace
{

using namespace mozjs;

JSClassOps jsOps = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    Utils::FinalizeJsObject,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};

JSClass jsClass = {
    "Utils",
    kDefaultClassFlags,
    &jsOps
};

MJS_DEFINE_JS_FN_FROM_NATIVE_WITH_OPT(CheckComponent, Utils::CheckComponent, Utils::CheckComponentWithOpt, 1);
MJS_DEFINE_JS_FN_FROM_NATIVE(CheckFont, Utils::CheckFont);
MJS_DEFINE_JS_FN_FROM_NATIVE(ColourPicker, Utils::ColourPicker);
MJS_DEFINE_JS_FN_FROM_NATIVE(DetectCharset, Utils::DetectCharset);
MJS_DEFINE_JS_FN_FROM_NATIVE(DownloadFileAsync, Utils::DownloadFileAsync);
MJS_DEFINE_JS_FN_FROM_NATIVE(EditTextFile, Utils::EditTextFile);
MJS_DEFINE_JS_FN_FROM_NATIVE(FileExists, Utils::FileExists);
MJS_DEFINE_JS_FN_FROM_NATIVE(FileTest, Utils::FileTest);
MJS_DEFINE_JS_FN_FROM_NATIVE(FormatDuration, Utils::FormatDuration);
MJS_DEFINE_JS_FN_FROM_NATIVE(FormatFileSize, Utils::FormatFileSize);
MJS_DEFINE_JS_FN_FROM_NATIVE_WITH_OPT(GetAlbumArtAsync, Utils::GetAlbumArtAsync, Utils::GetAlbumArtAsyncWithOpt, 4);
MJS_DEFINE_JS_FN_FROM_NATIVE_WITH_OPT(GetAlbumArtAsyncV2, Utils::GetAlbumArtAsyncV2, Utils::GetAlbumArtAsyncV2WithOpt, 4);
MJS_DEFINE_JS_FN_FROM_NATIVE_WITH_OPT(GetAlbumArtEmbedded, Utils::GetAlbumArtEmbedded, Utils::GetAlbumArtEmbeddedWithOpt, 1);
MJS_DEFINE_JS_FN_FROM_NATIVE_WITH_OPT(GetAlbumArtV2, Utils::GetAlbumArtV2, Utils::GetAlbumArtV2WithOpt, 2);
MJS_DEFINE_JS_FN_FROM_NATIVE(GetClipboardText, Utils::GetClipboardText);
MJS_DEFINE_JS_FN_FROM_NATIVE(GetFileSize, Utils::GetFileSize);
MJS_DEFINE_JS_FN_FROM_NATIVE(GetPackageInfo, Utils::GetPackageInfo);
MJS_DEFINE_JS_FN_FROM_NATIVE(GetPackagePath, Utils::GetPackagePath);
MJS_DEFINE_JS_FN_FROM_NATIVE(GetSysColour, Utils::GetSysColour);
MJS_DEFINE_JS_FN_FROM_NATIVE(GetSystemMetrics, Utils::GetSystemMetrics);
MJS_DEFINE_JS_FN_FROM_NATIVE_WITH_OPT(Glob, Utils::Glob, Utils::GlobWithOpt, 2);
MJS_DEFINE_JS_FN_FROM_NATIVE_WITH_OPT(InputBox, Utils::InputBox, Utils::InputBoxWithOpt, 2);
MJS_DEFINE_JS_FN_FROM_NATIVE(IsDirectory, Utils::IsDirectory);
MJS_DEFINE_JS_FN_FROM_NATIVE(IsFile, Utils::IsFile);
MJS_DEFINE_JS_FN_FROM_NATIVE(IsKeyPressed, Utils::IsKeyPressed);
MJS_DEFINE_JS_FN_FROM_NATIVE(MapString, Utils::MapString);
MJS_DEFINE_JS_FN_FROM_NATIVE(PathWildcardMatch, Utils::PathWildcardMatch);
MJS_DEFINE_JS_FN_FROM_NATIVE_WITH_OPT(ReadINI, Utils::ReadINI, Utils::ReadINIWithOpt, 1);
MJS_DEFINE_JS_FN_FROM_NATIVE_WITH_OPT(ReadTextFile, Utils::ReadTextFile, Utils::ReadTextFileWithOpt, 1);
MJS_DEFINE_JS_FN_FROM_NATIVE(SetClipboardText, Utils::SetClipboardText);
MJS_DEFINE_JS_FN_FROM_NATIVE_WITH_OPT(ShowHtmlDialog, Utils::ShowHtmlDialog, Utils::ShowHtmlDialogWithOpt, 1);
MJS_DEFINE_JS_FN_FROM_NATIVE(SplitFilePath, Utils::SplitFilePath);
MJS_DEFINE_JS_FN_FROM_NATIVE(WriteINI, Utils::WriteINI);
MJS_DEFINE_JS_FN_FROM_NATIVE_WITH_OPT(WriteTextFile, Utils::WriteTextFile, Utils::WriteTextFileWithOpt, 1);

constexpr auto jsFunctions = std::to_array<JSFunctionSpec>(
    {
        JS_FN("CheckComponent", CheckComponent, 1, kDefaultPropsFlags),
        JS_FN("CheckFont", CheckFont, 1, kDefaultPropsFlags),
        JS_FN("ColourPicker", ColourPicker, 2, kDefaultPropsFlags),
        JS_FN("DetectCharset", DetectCharset, 1, kDefaultPropsFlags),
        JS_FN("DownloadFileAsync", DownloadFileAsync, 2, kDefaultPropsFlags),
        JS_FN("EditTextFile", ::EditTextFile, 2, kDefaultPropsFlags),
        JS_FN("FileExists", FileExists, 1, kDefaultPropsFlags),
        JS_FN("FileTest", FileTest, 2, kDefaultPropsFlags),
        JS_FN("FormatDuration", FormatDuration, 1, kDefaultPropsFlags),
        JS_FN("FormatFileSize", FormatFileSize, 1, kDefaultPropsFlags),
        JS_FN("GetAlbumArtAsync", GetAlbumArtAsync, 2, kDefaultPropsFlags),
        JS_FN("GetAlbumArtAsyncV2", GetAlbumArtAsyncV2, 2, kDefaultPropsFlags),
        JS_FN("GetAlbumArtEmbedded", GetAlbumArtEmbedded, 1, kDefaultPropsFlags),
        JS_FN("GetAlbumArtV2", GetAlbumArtV2, 1, kDefaultPropsFlags),
        JS_FN("GetClipboardText", GetClipboardText, 0, kDefaultPropsFlags),
        JS_FN("GetFileSize", GetFileSize, 1, kDefaultPropsFlags),
        JS_FN("GetPackageInfo", GetPackageInfo, 1, kDefaultPropsFlags),
        JS_FN("GetPackagePath", GetPackagePath, 1, kDefaultPropsFlags),
        JS_FN("GetSysColour", GetSysColour, 1, kDefaultPropsFlags),
        JS_FN("GetSystemMetrics", GetSystemMetrics, 1, kDefaultPropsFlags),
        JS_FN("Glob", Glob, 1, kDefaultPropsFlags),
        JS_FN("InputBox", InputBox, 3, kDefaultPropsFlags),
        JS_FN("IsDirectory", IsDirectory, 1, kDefaultPropsFlags),
        JS_FN("IsFile", IsFile, 1, kDefaultPropsFlags),
        JS_FN("IsKeyPressed", IsKeyPressed, 1, kDefaultPropsFlags),
        JS_FN("MapString", MapString, 3, kDefaultPropsFlags),
        JS_FN("PathWildcardMatch", PathWildcardMatch, 2, kDefaultPropsFlags),
        JS_FN("ReadINI", ReadINI, 3, kDefaultPropsFlags),
        JS_FN("ReadTextFile", ReadTextFile, 1, kDefaultPropsFlags),
        JS_FN("SetClipboardText", SetClipboardText, 1, kDefaultPropsFlags),
        JS_FN("ShowHtmlDialog", ShowHtmlDialog, 3, kDefaultPropsFlags),
        JS_FN("SplitFilePath", SplitFilePath, 1, kDefaultPropsFlags),
        JS_FN("WriteINI", WriteINI, 4, kDefaultPropsFlags),
        JS_FN("WriteTextFile", WriteTextFile, 2, kDefaultPropsFlags),
        JS_FS_END,
    });

MJS_DEFINE_JS_FN_FROM_NATIVE(get_Version, Utils::get_Version)

constexpr auto jsProperties = std::to_array<JSPropertySpec>(
    {
        JS_PSG("Version", get_Version, kDefaultPropsFlags),
        JS_PS_END,
    });

} // namespace

namespace mozjs
{

const JSClass Utils::JsClass = jsClass;
const JSFunctionSpec* Utils::JsFunctions = jsFunctions.data();
const JSPropertySpec* Utils::JsProperties = jsProperties.data();

Utils::Utils(JSContext* cx)
    : pJsCtx_(cx)
{
}

std::unique_ptr<Utils>
Utils::CreateNative(JSContext* cx)
{
    return std::unique_ptr<Utils>(new Utils(cx));
}

size_t Utils::GetInternalSize()
{
    return 0;
}

bool Utils::CheckComponent(const std::string& name, bool is_dll) const
{
    pfc::string8_fast temp;
    for (service_enum_t<componentversion> e; !e.finished(); ++e)
    {
        auto cv = e.get();
        if (is_dll)
        {
            cv->get_file_name(temp);
        }
        else
        {
            cv->get_component_name(temp);
        }

        if (temp.c_str() == name)
        {
            return true;
        }
    }

    return false;
}

bool Utils::CheckComponentWithOpt(size_t optArgCount, const std::string& name, bool is_dll) const
{
    switch (optArgCount)
    {
    case 0:
        return CheckComponent(name, is_dll);
    case 1:
        return CheckComponent(name);
    default:
        throw qwr::QwrException("Internal error: invalid number of optional arguments specified: {}", optArgCount);
    }
}

bool Utils::CheckFont(const std::wstring& name) const
{
    Gdiplus::InstalledFontCollection font_collection;
    const int count = font_collection.GetFamilyCount();
    std::vector<Gdiplus::FontFamily> font_families(count);

    int recv;
    Gdiplus::Status gdiRet = font_collection.GetFamilies(count, font_families.data(), &recv);
    qwr::error::CheckGdi(gdiRet, "GetFamilies");
    qwr::QwrException::ExpectTrue(recv == count, "Internal error: GetFamilies numSought != numFound");

    std::array<wchar_t, LF_FACESIZE> family_name_eng{};
    std::array<wchar_t, LF_FACESIZE> family_name_loc{};
    const auto it = ranges::find_if(font_families, [&family_name_eng, &family_name_loc, &name](const auto& fontFamily) {
        Gdiplus::Status gdiRet = fontFamily.GetFamilyName(family_name_eng.data(), MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
        qwr::error::CheckGdi(gdiRet, "GetFamilyName");

        gdiRet = fontFamily.GetFamilyName(family_name_loc.data());
        qwr::error::CheckGdi(gdiRet, "GetFamilyName");

        return (!_wcsicmp(name.c_str(), family_name_loc.data())
                 || !_wcsicmp(name.c_str(), family_name_eng.data()));
    });

    return (it != font_families.cend());
}

uint32_t Utils::ColourPicker(uint32_t, uint32_t default_colour)
{
    static std::array<COLORREF, 16> colours{};
    const HWND hPanel = GetPanelHwndForCurrentGlobal(pJsCtx_);
    qwr::QwrException::ExpectTrue(hPanel, "Method called before fb2k was initialized completely");

    auto colour = smp::colour::ArgbToColorref(default_colour);
    uChooseColor(&colour, hPanel, colours.data());
    return smp::colour::ColorrefToArgb(colour);
}

uint32_t Utils::DetectCharset(const std::wstring& path) const
{
    const auto cleanedPath = fs::path(path).lexically_normal();

    return static_cast<uint32_t>(qwr::file::DetectFileCharset(cleanedPath));
}

void Utils::DownloadFileAsync(const std::string& url, const std::wstring& path)
{
    const auto wnd = GetPanelHwndForCurrentGlobal(pJsCtx_);
    auto task = fb2k::service_new<::DownloadFileAsync>(wnd, url, path);
    fb2k::cpuThreadPool::get()->runSingle(task);
}

void Utils::EditTextFile(const std::wstring& path)
{
    const HWND hPanel = GetPanelHwndForCurrentGlobal(pJsCtx_);
    qwr::QwrException::ExpectTrue(hPanel, "Method called before fb2k was initialized completely");

    if (!modal_dialog_scope::can_create())
    {
        return;
    }

    modal_dialog_scope scope(hPanel);

    // TODO: add options - editor_path, is_modal
    smp::EditTextFile(hPanel, fs::path{ path }, false, false);
}

bool Utils::FileExists(const std::wstring& path) const
{
    std::error_code ec;
    return fs::exists(path, ec);
}

JS::Value Utils::FileTest(const std::wstring& path, const std::wstring& mode)
{
    if (L"e" == mode) // exists
    {
        JS::RootedValue jsValue(pJsCtx_);
        convert::to_js::ToValue(pJsCtx_, FileExists(path), &jsValue);
        return jsValue;
    }
    else if (L"s" == mode)
    {
        JS::RootedValue jsValue(pJsCtx_);
        convert::to_js::ToValue(pJsCtx_, GetFileSize(path), &jsValue);
        return jsValue;
    }
    else if (L"d" == mode)
    {
        JS::RootedValue jsValue(pJsCtx_);
        convert::to_js::ToValue(pJsCtx_, IsDirectory(path), &jsValue);
        return jsValue;
    }
    else if (L"split" == mode)
    {
        return SplitFilePath(path);
    }
    else if (L"chardet" == mode)
    {
        JS::RootedValue jsValue(pJsCtx_);
        convert::to_js::ToValue(pJsCtx_, DetectCharset(path), &jsValue);
        return jsValue;
    }
    else
    {
        throw qwr::QwrException("Invalid value of mode argument: '{}'", qwr::unicode::ToU8(mode));
    }
}

std::string Utils::FormatDuration(double p) const
{
    return std::string(pfc::format_time_ex(p, 0));
}

std::string Utils::FormatFileSize(uint64_t p) const
{
    return std::string(pfc::format_file_size_short(p));
}

void Utils::GetAlbumArtAsync(uint32_t hWnd, JsFbMetadbHandle* handle, uint32_t art_id, bool need_stub, bool only_embed, bool no_load)
{
    (void)hWnd;
    const HWND hPanel = GetPanelHwndForCurrentGlobal(pJsCtx_);
    qwr::QwrException::ExpectTrue(hPanel, "Method called before fb2k was initialized completely");

    qwr::QwrException::ExpectTrue(handle, "handle argument is null");

    smp::art::GetAlbumArtAsync(hPanel, handle->GetHandle(), smp::art::LoadingOptions{ art_id, need_stub, only_embed, no_load });
}

void Utils::GetAlbumArtAsyncWithOpt(size_t optArgCount, uint32_t hWnd, JsFbMetadbHandle* handle, uint32_t art_id, bool need_stub, bool only_embed, bool no_load)
{
    switch (optArgCount)
    {
    case 0:
        return GetAlbumArtAsync(hWnd, handle, art_id, need_stub, only_embed, no_load);
    case 1:
        return GetAlbumArtAsync(hWnd, handle, art_id, need_stub, only_embed);
    case 2:
        return GetAlbumArtAsync(hWnd, handle, art_id, need_stub);
    case 3:
        return GetAlbumArtAsync(hWnd, handle, art_id);
    case 4:
        return GetAlbumArtAsync(hWnd, handle);
    default:
        throw qwr::QwrException("Internal error: invalid number of optional arguments specified: {}", optArgCount);
    }
}

JSObject* Utils::GetAlbumArtAsyncV2(uint32_t hWnd, JsFbMetadbHandle* handle, uint32_t art_id, bool need_stub, bool only_embed, bool no_load)
{
    (void)hWnd;
    const HWND hPanel = GetPanelHwndForCurrentGlobal(pJsCtx_);
    qwr::QwrException::ExpectTrue(hPanel, "Method called before fb2k was initialized completely");
    qwr::QwrException::ExpectTrue(handle, "handle argument is null");

    return mozjs::art::GetAlbumArtPromise(pJsCtx_, hPanel, handle->GetHandle(), smp::art::LoadingOptions{ art_id, need_stub, only_embed, no_load });
}

JSObject* Utils::GetAlbumArtAsyncV2WithOpt(size_t optArgCount, uint32_t hWnd, JsFbMetadbHandle* handle, uint32_t art_id, bool need_stub, bool only_embed, bool no_load)
{
    switch (optArgCount)
    {
    case 0:
        return GetAlbumArtAsyncV2(hWnd, handle, art_id, need_stub, only_embed, no_load);
    case 1:
        return GetAlbumArtAsyncV2(hWnd, handle, art_id, need_stub, only_embed);
    case 2:
        return GetAlbumArtAsyncV2(hWnd, handle, art_id, need_stub);
    case 3:
        return GetAlbumArtAsyncV2(hWnd, handle, art_id);
    case 4:
        return GetAlbumArtAsyncV2(hWnd, handle);
    default:
        throw qwr::QwrException("Internal error: invalid number of optional arguments specified: {}", optArgCount);
    }
}

JSObject* Utils::GetAlbumArtEmbedded(const std::string& rawpath, uint32_t art_id)
{
    std::unique_ptr<Gdiplus::Bitmap> artImage(smp::art::GetBitmapFromEmbeddedData(rawpath, art_id));
    if (!artImage)
    { // Not an error: no art found
        return nullptr;
    }

    return JsGdiBitmap::CreateJs(pJsCtx_, std::move(artImage));
}

JSObject* Utils::GetAlbumArtEmbeddedWithOpt(size_t optArgCount, const std::string& rawpath, uint32_t art_id)
{
    switch (optArgCount)
    {
    case 0:
        return GetAlbumArtEmbedded(rawpath, art_id);
    case 1:
        return GetAlbumArtEmbedded(rawpath);
    default:
        throw qwr::QwrException("Internal error: invalid number of optional arguments specified: {}", optArgCount);
    }
}

JSObject* Utils::GetAlbumArtV2(JsFbMetadbHandle* handle, uint32_t art_id, bool need_stub)
{
    qwr::QwrException::ExpectTrue(handle, "handle argument is null");

    std::unique_ptr<Gdiplus::Bitmap> artImage(smp::art::GetBitmapFromMetadb(handle->GetHandle(), smp::art::LoadingOptions{ art_id, need_stub, false, false }, nullptr));
    if (!artImage)
    { // Not an error: no art found
        return nullptr;
    }

    return JsGdiBitmap::CreateJs(pJsCtx_, std::move(artImage));
}

JSObject* Utils::GetAlbumArtV2WithOpt(size_t optArgCount, JsFbMetadbHandle* handle, uint32_t art_id, bool need_stub)
{
    switch (optArgCount)
    {
    case 0:
        return GetAlbumArtV2(handle, art_id, need_stub);
    case 1:
        return GetAlbumArtV2(handle, art_id);
    case 2:
        return GetAlbumArtV2(handle);
    default:
        throw qwr::QwrException("Internal error: invalid number of optional arguments specified: {}", optArgCount);
    }
}

std::string Utils::GetClipboardText() const
{
    pfc::string8 text;
    uGetClipboardString(text);
    return std::string(text);
}

uint64_t Utils::GetFileSize(const std::wstring& path) const
{
    if (fs::is_regular_file(path))
    {
        return fs::file_size(path);
    }

    return {};
}

JSObject* Utils::GetPackageInfo(const std::string& packageId) const
{
    const auto packagePathOpt = config::FindPackage(packageId);
    if (!packagePathOpt)
    {
        return nullptr;
    }

    const auto settings = config::GetPackageSettingsFromPath(*packagePathOpt);

    JS::RootedObject jsDirs(pJsCtx_, JS_NewPlainObject(pJsCtx_));
    AddProperty(pJsCtx_, jsDirs, "Root", config::GetPackagePath(settings).wstring());
    AddProperty(pJsCtx_, jsDirs, "Assets", config::GetPackageAssetsDir(settings).wstring());
    AddProperty(pJsCtx_, jsDirs, "Scripts", config::GetPackageScriptsDir(settings).wstring());
    AddProperty(pJsCtx_, jsDirs, "Storage", config::GetPackageStorageDir(settings).wstring());

    JS::RootedObject jsObject(pJsCtx_, JS_NewPlainObject(pJsCtx_));
    AddProperty(pJsCtx_, jsObject, "Directories", static_cast<JS::HandleObject>(jsDirs));
    AddProperty(pJsCtx_, jsObject, "Version", settings.scriptVersion);

    return jsObject;
}

std::string Utils::GetPackagePath(const std::string& packageId) const
{
    const auto packagePathOpt = config::FindPackage(packageId);
    qwr::QwrException::ExpectTrue(packagePathOpt.has_value(), "Unknown package: {}", packageId);

    return packagePathOpt->u8string();
}

uint32_t Utils::GetSysColour(uint32_t index) const
{
    const auto hBrush = ::GetSysColorBrush(index); ///< no need to call DeleteObject here
    qwr::QwrException::ExpectTrue(hBrush, "Invalid color index: {}", index);

    return smp::colour::ColorrefToArgb(::GetSysColor(index));
}

uint32_t Utils::GetSystemMetrics(uint32_t index) const
{
    return ::GetSystemMetrics(index);
}

JS::Value Utils::Glob(const std::wstring& pattern, uint32_t exc_mask, uint32_t inc_mask)
{
    std::vector<std::wstring> files;
    WIN32_FIND_DATA data{};
    auto hFindFile = wil::unique_hfind(FindFirstFileW(pattern.data(), &data));

    if (hFindFile)
    {
        const auto folder = std::filesystem::path(pattern).parent_path().native() + std::filesystem::path::preferred_separator;

        while (true)
        {
            const DWORD attr = data.dwFileAttributes;

            if (WI_IsAnyFlagSet(attr, inc_mask) && !WI_IsAnyFlagSet(attr, exc_mask))
            {
                files.emplace_back(folder + data.cFileName);
            }

            if (!FindNextFileW(hFindFile.get(), &data))
                break;
        }
    }

    std::ranges::sort(files, CmpW());

    JS::RootedValue jsValue(pJsCtx_);
    convert::to_js::ToArrayValue(pJsCtx_, files, &jsValue);
    return jsValue;
}

JS::Value Utils::GlobWithOpt(size_t optArgCount, const std::wstring& pattern, uint32_t exc_mask, uint32_t inc_mask)
{
    switch (optArgCount)
    {
    case 0:
        return Glob(pattern, exc_mask, inc_mask);
    case 1:
        return Glob(pattern, exc_mask);
    case 2:
        return Glob(pattern);
    default:
        throw qwr::QwrException("Internal error: invalid number of optional arguments specified: {}", optArgCount);
    }
}

std::string Utils::InputBox(uint32_t hWnd, const std::string& prompt, const std::string& caption, const std::string& def, bool error_on_cancel)
{
    (void)hWnd;
    const HWND hPanel = GetPanelHwndForCurrentGlobal(pJsCtx_);
    qwr::QwrException::ExpectTrue(hPanel, "Method called before fb2k was initialized completely");

    if (modal_dialog_scope::can_create())
    {
        modal_dialog_scope scope(hPanel);

        smp::ui::CInputBox dlg(prompt.c_str(), caption.c_str(), def.c_str());
        int status = dlg.DoModal(hPanel);
        if (status == IDCANCEL && error_on_cancel)
        {
            throw qwr::QwrException("Dialog window was closed");
        }

        if (status == IDOK)
        {
            return dlg.GetValue();
        }
    }

    return def;
}

std::string Utils::InputBoxWithOpt(size_t optArgCount, uint32_t hWnd, const std::string& prompt, const std::string& caption, const std::string& def, bool error_on_cancel)
{
    switch (optArgCount)
    {
    case 0:
        return InputBox(hWnd, prompt, caption, def, error_on_cancel);
    case 1:
        return InputBox(hWnd, prompt, caption, def);
    case 2:
        return InputBox(hWnd, prompt, caption);
    default:
        throw qwr::QwrException("Internal error: invalid number of optional arguments specified: {}", optArgCount);
    }
}

bool Utils::IsDirectory(const std::wstring& path) const
{
    std::error_code ec;
    return fs::is_directory(path, ec);
}

bool Utils::IsFile(const std::wstring& path) const
{
    std::error_code ec;
    return fs::is_regular_file(path, ec);
}

bool Utils::IsKeyPressed(uint32_t vkey) const
{
    return ::IsKeyPressed(vkey);
}

std::wstring Utils::MapString(const std::wstring& str, uint32_t lcid, uint32_t flags)
{
    // WinAPI is weird: 0 - error (with LastError), > 0 - characters required
    int iRet = LCIDToLocaleName(lcid, nullptr, 0, LOCALE_ALLOW_NEUTRAL_NAMES);
    qwr::error::CheckWinApi(iRet, "LCIDToLocaleName(nullptr)");

    std::wstring localeName(iRet, '\0');
    iRet = LCIDToLocaleName(lcid, localeName.data(), localeName.size(), LOCALE_ALLOW_NEUTRAL_NAMES);
    qwr::error::CheckWinApi(iRet, "LCIDToLocaleName(data)");

    std::optional<NLSVERSIONINFOEX> versionInfo;
    try
    {
        if (_WIN32_WINNT_WIN7 > GetWindowsVersionCode())
        {
            NLSVERSIONINFOEX tmpVersionInfo{};
            BOOL bRet = GetNLSVersionEx(COMPARE_STRING, localeName.c_str(), &tmpVersionInfo);
            qwr::error::CheckWinApi(bRet, "GetNLSVersionEx");

            versionInfo = tmpVersionInfo;
        }
    }
    catch (const std::exception&)
    {
    }

    auto* pVersionInfo = reinterpret_cast<NLSVERSIONINFO*>(versionInfo ? &(*versionInfo) : nullptr);

    iRet = LCMapStringEx(localeName.c_str(), flags, str.c_str(), str.length() + 1, nullptr, 0, pVersionInfo, nullptr, 0);
    qwr::error::CheckWinApi(iRet, "LCMapStringEx(nullptr)");

    std::wstring dst(iRet, '\0');
    iRet = LCMapStringEx(localeName.c_str(), flags, str.c_str(), str.length() + 1, dst.data(), dst.size(), pVersionInfo, nullptr, 0);
    qwr::error::CheckWinApi(iRet, "LCMapStringEx(data)");

    dst.resize(wcslen(dst.c_str()));
    return dst;
}

bool Utils::PathWildcardMatch(const std::wstring& pattern, const std::wstring& str)
{
    return PathMatchSpec(str.c_str(), pattern.c_str());
}

std::wstring Utils::ReadINI(const std::wstring& filename, const std::wstring& section, const std::wstring& key, const std::wstring& defaultval)
{
    // WinAPI is weird: 0 - error (with LastError), > 0 - characters required
    std::wstring dst(MAX_PATH, '\0');
    int iRet = GetPrivateProfileString(section.c_str(), key.c_str(), defaultval.c_str(), dst.data(), dst.size(), filename.c_str());
    // TODO v2: Uncomment error checking
    // qwr::error::CheckWinApi((iRet || (NO_ERROR == GetLastError())), "GetPrivateProfileString(nullptr)");

    if (!iRet && (NO_ERROR != GetLastError()))
    {
        dst = defaultval;
    }
    else
    {
        dst.resize(wcslen(dst.c_str()));
    }

    return dst;
}

std::wstring Utils::ReadINIWithOpt(size_t optArgCount, const std::wstring& filename, const std::wstring& section, const std::wstring& key, const std::wstring& defaultval)
{
    switch (optArgCount)
    {
    case 0:
        return ReadINI(filename, section, key, defaultval);
    case 1:
        return ReadINI(filename, section, key);
    default:
        throw qwr::QwrException("Internal error: invalid number of optional arguments specified: {}", optArgCount);
    }
}

std::wstring Utils::ReadTextFile(const std::wstring& filePath, uint32_t codepage)
{
    return qwr::file::ReadFileW(filePath, codepage);
}

std::wstring Utils::ReadTextFileWithOpt(size_t optArgCount, const std::wstring& filePath, uint32_t codepage)
{
    switch (optArgCount)
    {
    case 0:
        return ReadTextFile(filePath, codepage);
    case 1:
        return ReadTextFile(filePath);
    default:
        throw qwr::QwrException("Internal error: invalid number of optional arguments specified: {}", optArgCount);
    }
}

void Utils::SetClipboardText(const std::string& text)
{
    uSetClipboardString(text.c_str());
}

JS::Value Utils::ShowHtmlDialog(uint32_t hWnd, const std::wstring& htmlCode, JS::HandleValue options)
{
    (void)hWnd;
    const HWND hPanel = GetPanelHwndForCurrentGlobal(pJsCtx_);
    qwr::QwrException::ExpectTrue(hPanel, "Method called before fb2k was initialized completely");

    if (modal_dialog_scope::can_create())
    {
        modal_dialog_scope scope(hPanel);

        smp::ui::CDialogHtml dlg(pJsCtx_, htmlCode, options);
        int iRet = dlg.DoModal(hPanel);
        if (-1 == iRet || IDABORT == iRet)
        {
            if (JS_IsExceptionPending(pJsCtx_))
            {
                throw JsException();
            }
            else
            {
                throw qwr::QwrException("DoModal failed: {}", iRet);
            }
        }
    }

    // TODO: placeholder for modeless
    return JS::UndefinedValue();
}

JS::Value Utils::ShowHtmlDialogWithOpt(size_t optArgCount, uint32_t hWnd, const std::wstring& htmlCode, JS::HandleValue options)
{
    switch (optArgCount)
    {
    case 0:
        return ShowHtmlDialog(hWnd, htmlCode, options);
    case 1:
        return ShowHtmlDialog(hWnd, htmlCode);
    default:
        throw qwr::QwrException("Internal error: invalid number of optional arguments specified: {}", optArgCount);
    }
}

JS::Value Utils::SplitFilePath(const std::wstring& path)
{
    const auto cleanedPath = fs::path(path).lexically_normal();

    std::vector<std::wstring> out(3);
    if (PathIsFileSpec(cleanedPath.filename().c_str()))
    {
        out[0] = cleanedPath.parent_path() / "";
        out[1] = cleanedPath.stem();
        out[2] = cleanedPath.extension();
    }
    else
    {
        out[0] = cleanedPath / "";
    }

    JS::RootedValue jsValue(pJsCtx_);
    convert::to_js::ToArrayValue(pJsCtx_, out, &jsValue);

    return jsValue;
}

bool Utils::WriteINI(const std::wstring& filename, const std::wstring& section, const std::wstring& key, const std::wstring& val)
{
    return WritePrivateProfileString(section.c_str(), key.c_str(), val.c_str(), filename.c_str());
}

bool Utils::WriteTextFile(const std::wstring& filename, const std::string& content, bool write_bom)
{
    qwr::QwrException::ExpectTrue(!filename.empty(), "Invalid filename");

    try
    {
        qwr::file::WriteFile(filename, content, write_bom);
        return true;
    }
    catch (const qwr::QwrException&)
    {
        return false;
    }
}

bool Utils::WriteTextFileWithOpt(size_t optArgCount, const std::wstring& filename, const std::string& content, bool write_bom)
{
    switch (optArgCount)
    {
    case 0:
        return WriteTextFile(filename, content, write_bom);
    case 1:
        return WriteTextFile(filename, content);
    default:
        throw qwr::QwrException("Internal error: invalid number of optional arguments specified: {}", optArgCount);
    }
}

std::string Utils::get_Version() const
{
    return SMP_VERSION;
}

} // namespace mozjs
