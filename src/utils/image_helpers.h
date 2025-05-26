#pragma once

namespace smp::image
{

[[nodiscard]] std::tuple<uint32_t, uint32_t> GetResizedImageSize(
    const std::tuple<uint32_t, uint32_t>& currentDimension,
    const std::tuple<uint32_t, uint32_t>& maxDimensions) noexcept;

/// @remark WIC loads images 'eagerly' which makes loading operation slower by x100, so it should be used only as a last resort
[[nodiscard]] std::unique_ptr<Gdiplus::Bitmap> LoadImageWithWIC(IStream* pStream);

} // namespace smp::image
