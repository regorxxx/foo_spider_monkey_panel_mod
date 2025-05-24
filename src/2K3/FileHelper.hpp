#pragma once

class FileHelper
{
public:
	FileHelper(const std::filesystem::path& path);
	FileHelper(std::string_view path);

	static bool rename(const std::filesystem::path& from, const std::filesystem::path& to);
	static uint32_t get_stream_size(IStream* stream);

	HRESULT read(wil::com_ptr<IStream>& stream);
	bool copy_file(const std::filesystem::path& to, bool overwrite);
	bool copy_folder(const std::filesystem::path& to, bool overwrite, bool recur);
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
