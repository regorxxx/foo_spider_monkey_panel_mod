#pragma once
#define _WIN32_WINNT _WIN32_WINNT_WIN7
#define WINVER _WIN32_WINNT_WIN7
#define NOMINMAX

#include <atomic>
#include <cassert>
#include <charconv>
#include <cwctype>
#include <filesystem>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <span>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <unordered_map>

#include <WinSock2.h>
#include <windows.h>

#include <ShlDisp.h>
#include <exdisp.h>
#include <shobjidl_core.h>
// Generates wrappers for COM listed above
#include <ComDef.h>

// foobar2000 SDK
#pragma warning(push, 0)
#include <foobar2000/SDK/foobar2000.h>
#pragma warning(pop)

// fmt
#define FMT_HEADER_ONLY
#include <fmt/format.h>
/// wchar_t support
#include <fmt/xchar.h>

// range v3
#include <range/v3/all.hpp>

// wil
#include <wil/resource.h>

// Additional PFC wrappers
#include <qwr/pfc_helpers_cnt.h>

// Unicode converters
#include <qwr/unicode.h>

#include <qwr/qwr_exception.h>
