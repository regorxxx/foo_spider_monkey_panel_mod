#pragma once

class FileHelper
{
public:
	FileHelper(std::wstring_view path);
	FileHelper(std::string_view path);

	static bool rename(std::wstring_view from, std::wstring_view to);
	static uint32_t get_stream_size(IStream* stream);

	HRESULT read(wil::com_ptr<IStream>& stream);
	bool copy_file(std::wstring_view to, bool overwrite);
	bool copy_folder(std::wstring_view to, bool overwrite, bool recur);
	bool create_folder();
	bool is_file();
	bool is_folder();
	bool remove();
	bool write(const void* data, size_t size);
	uint64_t file_size();
	uint64_t last_modified();

private:
	std::filesystem::copy_options create_options(bool overwrite, bool recur = false);

	std::filesystem::path m_path;
};
