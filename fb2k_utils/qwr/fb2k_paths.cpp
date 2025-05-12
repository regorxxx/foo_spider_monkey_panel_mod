#include <stdafx.h>

#include "fb2k_paths.h"

#include <filesystem>

namespace fs = std::filesystem;

namespace qwr::path
{

fs::path Component()
{
    pfc::string8_fast path;
    uGetModuleFileName(core_api::get_my_instance(), path);
    const auto wpath = qwr::unicode::ToWide(path);

    return fs::path(wpath).parent_path().lexically_normal();
}

fs::path Foobar2000()
{
    pfc::string8_fast path;
    uGetModuleFileName(nullptr, path);
    const auto wpath = qwr::unicode::ToWide(path);

    return fs::path(wpath).parent_path().lexically_normal();
}

fs::path Profile()
{
    const auto path = filesystem::g_get_native_path( core_api::get_profile_path());
    const auto wpath = qwr::unicode::ToWide(path);

    return fs::path(wpath).lexically_normal();
}

} // namespace qwr::path
