#include <stdafx.h>
#include <pfc/string-conv-lite.h>

#include "unicode.h"

namespace qwr
{
std::string ToU8(std::wstring_view src)
{
    return pfc::utf8FromWide(src.data(), src.length()).get_ptr();
}

std::wstring ToWide(std::string_view str)
{
    return pfc::wideFromUTF8(str.data(), str.length()).c_str();
}

std::wstring ToWide(const pfc::string_base& src)
{
    return pfc::wideFromUTF8(src, src.length()).c_str();
}

std::string ToU8_FromAcpToWide(std::string_view src)
{
    return ToU8(ToWide_FromAcp(src));
}

std::wstring ToWide_FromAcp(std::string_view src)
{
    if (src.empty())
    {
        return std::wstring{};
    }

    size_t stringLen = MultiByteToWideChar(CP_ACP, 0, src.data(), src.size(), nullptr, 0);
    std::wstring strVal;
    strVal.resize(stringLen);

    stringLen = MultiByteToWideChar(CP_ACP, 0, src.data(), src.size(), strVal.data(), strVal.size());
    strVal.resize(stringLen);

    return strVal;
}
} // namespace qwr
