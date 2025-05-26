#include <stdafx.h>
#include "image_helpers.h"
#include <utils/gdi_helpers.h>

namespace smp::image
{

std::tuple<uint32_t, uint32_t> GetResizedImageSize(
    const std::tuple<uint32_t, uint32_t>& currentDimension,
    const std::tuple<uint32_t, uint32_t>& maxDimensions) noexcept
{
    const auto& [maxWidth, maxHeight] = maxDimensions;
    const auto& [imgWidth, imgHeight] = currentDimension;

    uint32_t newWidth;
    uint32_t newHeight;
    if (imgWidth <= maxWidth && imgHeight <= maxHeight)
    {
        newWidth = imgWidth;
        newHeight = imgHeight;
    }
    else
    {
        const double imgRatio = static_cast<double>(imgHeight) / imgWidth;
        const double constraintsRatio = static_cast<double>(maxHeight) / maxWidth;
        if (imgRatio > constraintsRatio)
        {
            newHeight = maxHeight;
            newWidth = lround(newHeight / imgRatio);
        }
        else
        {
            newWidth = maxWidth;
            newHeight = lround(newWidth * imgRatio);
        }
    }

    return std::make_tuple(newWidth, newHeight);
}

std::unique_ptr<Gdiplus::Bitmap> LoadWithWIC(IStream* stream)
{
    auto factory = wil::CoCreateInstance<IWICImagingFactory>(CLSID_WICImagingFactory);
    if (!factory)
        return nullptr;

    wil::com_ptr<IWICBitmapDecoder> decoder;
    wil::com_ptr<IWICBitmapFrameDecode> frame;
    wil::com_ptr<IWICBitmapSource> source;

    if FAILED(factory->CreateDecoderFromStream(stream, nullptr, WICDecodeMetadataCacheOnDemand, &decoder))
        return nullptr;

    if FAILED(decoder->GetFrame(0, &frame))
        return nullptr;

    if FAILED(WICConvertBitmapSource(GUID_WICPixelFormat32bppPBGRA, frame.get(), &source))
        return nullptr;

    uint32_t w{}, h{};
    if FAILED(source->GetSize(&w, &h))
        return nullptr;

    static constexpr auto gdiFormat = PixelFormat32bppPARGB;
    auto bitmap = std::make_unique<Gdiplus::Bitmap>(w, h, gdiFormat);

    Gdiplus::Rect rect{ 0, 0, static_cast<INT>(w), static_cast<INT>(h) };
    Gdiplus::BitmapData bmpdata{};
    auto gdiStatus = bitmap->LockBits(&rect, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite, gdiFormat, &bmpdata);
    if (gdiStatus != Gdiplus::Ok)
        return nullptr;

    {
        // unlock bits before returning
        auto autoDstBits = wil::scope_exit([&bitmap, &bmpdata] { bitmap->UnlockBits(&bmpdata); });

        if FAILED(source->CopyPixels(nullptr, bmpdata.Stride, bmpdata.Stride * bmpdata.Height, static_cast<uint8_t*>(bmpdata.Scan0)))
            return nullptr;
    }

    return bitmap;
}

std::unique_ptr<Gdiplus::Bitmap> Load(IStream* stream)
{
    auto bitmap = std::make_unique<Gdiplus::Bitmap>(stream, TRUE);
    if (smp::gdi::IsGdiPlusObjectValid(bitmap))
        return bitmap;

    return LoadWithWIC(stream);
}

} // namespace smp::image
