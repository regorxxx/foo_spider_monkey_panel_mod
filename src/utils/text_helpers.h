#pragma once

#include <list>
#include <string>

namespace smp::utils
{

[[nodiscard]] size_t GetTextHeight( HDC hdc, std::wstring_view text );
[[nodiscard]] size_t GetTextWidth( HDC hdc, std::wstring_view text, bool accurate = false );

struct WrappedTextLine
{
    std::wstring_view text;
    size_t width;
};
[[nodiscard]] std::vector<WrappedTextLine> WrapText( HDC hdc, const std::wstring& text, size_t maxWidth );

}

namespace CustomSort
{
    struct Item
    {
        pfc::wstringLite text;
        size_t index{};
    };

    using Order = pfc::array_t<size_t>;

    template <int32_t direction>
    static bool sort_compare(const Item& a, const Item& b)
    {
        const auto ret = direction * StrCmpLogicalW(a.text, b.text);

        if (ret == 0)
            return a.index < b.index;

        return ret < 0;
    }

    static Order order(size_t count)
    {
        Order sort_order;
        sort_order.set_size(count);
        std::iota(sort_order.begin(), sort_order.end(), size_t{});
        return sort_order;
    }

    static Order sort(pfc::array_t<Item>& items, int32_t direction = 1)
    {
        std::ranges::sort(items, direction > 0 ? sort_compare<1> : sort_compare<-1>);

        auto sort_order = order(items.get_count());
        std::ranges::transform(items, sort_order.begin(), [](const Item& item) { return item.index; });
        return sort_order;
    }
}
