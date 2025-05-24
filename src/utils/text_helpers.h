#pragma once

namespace smp::utils
{

[[nodiscard]] size_t GetTextHeight(HDC hdc, std::wstring_view text);
[[nodiscard]] size_t GetTextWidth(HDC hdc, std::wstring_view text, bool accurate = false);

struct WrappedTextLine
{
    std::wstring_view text;
    size_t width;
};
[[nodiscard]] std::vector<WrappedTextLine> WrapText(HDC hdc, const std::wstring& text, size_t maxWidth);

}
