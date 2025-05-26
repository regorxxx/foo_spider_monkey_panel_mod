#pragma once

namespace smp::image
{

[[nodiscard]] std::tuple<uint32_t, uint32_t> GetResizedImageSize(
    const std::tuple<uint32_t, uint32_t>& currentDimension,
    const std::tuple<uint32_t, uint32_t>& maxDimensions) noexcept;

[[nodiscard]] std::unique_ptr<Gdiplus::Bitmap> Load(IStream* stream);

} // namespace smp::image
