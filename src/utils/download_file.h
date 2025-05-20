#pragma once

class DownloadFileAsync : public fb2k::threadEntry
{
public:
	DownloadFileAsync(HWND wnd, std::string_view url, std::wstring_view path);

	void run() final;

private:
	bool write(const void* data, size_t size);

	HWND m_wnd;
	bool m_verify_image{};
	std::string m_url;
	std::wstring m_path;
};
