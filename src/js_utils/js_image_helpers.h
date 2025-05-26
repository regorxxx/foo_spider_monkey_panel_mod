#pragma once

namespace mozjs::image
{

/// @throw qwr::QwrException
/// @throw smp::JsException
[[nodiscard]] JSObject* GetImagePromise(JSContext* cx, HWND hWnd, const std::wstring& imagePath);

} // namespace mozjs::image
