#include <stdafx.h>

#include "image_helpers.h"

#include <2K3/FileHelper.hpp>
#include <events/event_dispatcher.h>
#include <events/event_js_callback.h>
#include <utils/gdi_helpers.h>
#include <utils/guid_helpers.h>
#include <utils/thread_pool_instance.h>

namespace
{

using namespace smp;

class LoadImageTask
{
public:
    LoadImageTask(HWND hNotifyWnd, uint32_t taskId, const std::wstring& imagePath);

    LoadImageTask(const LoadImageTask&) = delete;
    LoadImageTask& operator=(const LoadImageTask&) = delete;

    void operator()();

    [[nodiscard]] uint32_t GetTaskId() const;

private:
    void run();

private:
    HWND hNotifyWnd_;
    uint32_t taskId_{};
    std::wstring imagePath_;
};

LoadImageTask::LoadImageTask(HWND hNotifyWnd, uint32_t taskId, const std::wstring& imagePath)
    : hNotifyWnd_(hNotifyWnd)
    , taskId_(taskId)
    , imagePath_(imagePath)
{
}

void LoadImageTask::operator()()
{
    return run();
}

uint32_t LoadImageTask::GetTaskId() const
{
    return taskId_;
}

void LoadImageTask::run()
{
    const std::string path = file_path_display(qwr::ToU8(imagePath_).c_str()).get_ptr();

    EventDispatcher::Get().PutEvent(hNotifyWnd_,
                                     GenerateEvent_JsCallback(
                                         EventId::kInternalLoadImageDone,
                                         taskId_,
                                         image::LoadImage(imagePath_),
                                         path));
}

} // namespace

namespace smp::image
{

std::tuple<uint32_t, uint32_t>
GetResizedImageSize(const std::tuple<uint32_t, uint32_t>& currentDimension,
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

uint32_t LoadImageAsync(HWND hWnd, const std::wstring& imagePath)
{
    // This is performed on the main thread only, so it's all good
    static uint32_t g_taskId = 0;
    if (g_taskId == std::numeric_limits<uint32_t>::max())
    {
        g_taskId = 0;
    }

    auto task = std::make_shared<LoadImageTask>(hWnd, g_taskId++, imagePath);
    smp::GetThreadPoolInstance().AddTask([task] { std::invoke(*task); });

    return task->GetTaskId();
}

std::unique_ptr<Gdiplus::Bitmap> LoadImage(const std::wstring& imagePath)
{
    wil::com_ptr<IStream> stream;
    if FAILED(FileHelper(imagePath).read(stream))
        return nullptr;

    auto pImg = std::make_unique<Gdiplus::Bitmap>(stream.get(), TRUE);
    if (gdi::IsGdiPlusObjectValid(pImg))
        return pImg;

    return LoadImageWithWIC(stream.get());
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
