#include <stdafx.h>

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

std::unique_ptr<Gdiplus::Bitmap> LoadImageWithWIC(IStream* pStream)
{
    auto pFactory = wil::CoCreateInstance<IWICImagingFactory>(CLSID_WICImagingFactory);
    if (!pFactory)
        return nullptr;

    wil::com_ptr<IWICBitmapDecoder> pDecoder;
    wil::com_ptr<IWICBitmapFrameDecode> pFrame;
    wil::com_ptr<IWICBitmapSource> pSource;

    if FAILED(pFactory->CreateDecoderFromStream(pStream, nullptr, WICDecodeMetadataCacheOnDemand, &pDecoder))
        return nullptr;

    if FAILED(pDecoder->GetFrame(0, &pFrame))
        return nullptr;

    if FAILED(WICConvertBitmapSource(GUID_WICPixelFormat32bppPBGRA, pFrame.get(), &pSource))
        return nullptr;

    uint32_t w{}, h{};
    if FAILED(pSource->GetSize(&w, &h))
        return nullptr;

    static constexpr auto gdiFormat = PixelFormat32bppPARGB;
    auto pGdiBitmap = std::make_unique<Gdiplus::Bitmap>(w, h, gdiFormat);
    if (!pGdiBitmap || Gdiplus::Status::Ok != pGdiBitmap->GetLastStatus())
        return nullptr;

    Gdiplus::Rect rect{ 0, 0, static_cast<INT>(w), static_cast<INT>(h) };
    Gdiplus::BitmapData bmpdata{};
    auto gdiStatus = pGdiBitmap->LockBits(&rect, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite, gdiFormat, &bmpdata);
    if (gdiStatus != Gdiplus::Ok)
        return nullptr;

    {
        // unlock bits before returning
        auto autoDstBits = wil::scope_exit([&pGdiBitmap, &bmpdata] { pGdiBitmap->UnlockBits(&bmpdata); });

        if FAILED(pSource->CopyPixels(nullptr, bmpdata.Stride, bmpdata.Stride * bmpdata.Height, static_cast<uint8_t*>(bmpdata.Scan0)))
            return nullptr;
    }

    return pGdiBitmap;
}

} // namespace smp::image
