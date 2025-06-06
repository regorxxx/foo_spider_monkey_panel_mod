#pragma once

namespace qwr
{
std::wstring ToWide(const char*) = delete;
std::wstring ToWide_FromAcp(const char*) = delete;
std::string ToU8(const wchar_t*) = delete;
std::string ToU8_FromAcpToWide(const wchar_t*) = delete;

std::string ToU8(std::wstring_view src);
std::wstring ToWide(std::string_view src);
std::wstring ToWide(const pfc::string_base& src);

std::string ToU8_FromAcpToWide(std::string_view src);
std::wstring ToWide_FromAcp(std::string_view src);
} // namespace qwr
