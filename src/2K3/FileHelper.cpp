#include <stdafx.h>
#include "FileHelper.hpp"

namespace fs = std::filesystem;

FileHelper::FileHelper(std::wstring_view path) : m_path(path.data()) {}
FileHelper::FileHelper(std::string_view path) : m_path(qwr::unicode::ToWide(path)) {}

#pragma region static
bool FileHelper::rename(std::wstring_view from, std::wstring_view to)
{
	const auto fs_from = fs::path(from.data());
	const auto fs_to = fs::path(to.data());
	std::error_code ec;

	fs::rename(fs_from, fs_to, ec);
	return ec.value() == 0;
}

uint32_t FileHelper::get_stream_size(IStream* stream)
{
	STATSTG stats{};

	if FAILED(stream->Stat(&stats, STATFLAG_DEFAULT))
		return UINT_MAX;

	return stats.cbSize.LowPart;
}
#pragma endregion

HRESULT FileHelper::read(wil::com_ptr<IStream>& stream)
{
	static constexpr uint32_t max_image_size = 1024 * 1024 * 64; // 64MB
	RETURN_IF_FAILED(SHCreateStreamOnFileEx(m_path.c_str(), STGM_READ | STGM_SHARE_DENY_WRITE, GENERIC_READ, FALSE, nullptr, &stream));
	RETURN_HR_IF(E_INVALIDARG, get_stream_size(stream.get()) > max_image_size);
	return S_OK;
}

bool FileHelper::copy_file(std::wstring_view to, bool overwrite)
{
	if (!is_file())
		return false;

	const auto fs_to = fs::path(to.data());
	const auto options = create_options(overwrite);
	std::error_code ec;

	return fs::copy_file(m_path, fs_to, options, ec);
}

bool FileHelper::copy_folder(std::wstring_view to, bool overwrite, bool recur)
{
	if (!is_folder())
		return false;

	const auto fs_to = fs::path(to.data());
	const auto options = create_options(overwrite, recur);
	std::error_code ec;

	fs::copy(m_path, fs_to, options, ec);
	return ec.value() == 0;
}

bool FileHelper::create_folder()
{
	std::error_code ec;

	if (fs::is_directory(m_path, ec))
		return true;

	return fs::create_directories(m_path, ec);
}

bool FileHelper::is_file()
{
	std::error_code ec;
	return fs::is_regular_file(m_path, ec);
}

bool FileHelper::is_folder()
{
	std::error_code ec;
	return fs::is_directory(m_path, ec);
}

bool FileHelper::remove()
{
	std::error_code ec;
	return fs::remove(m_path, ec);
}

bool FileHelper::write(const void* data, size_t size)
{
	auto f = std::ofstream(m_path, std::ios::binary);

	if (!f.is_open())
		return false;

	return f.write((char*)data, size).good();
}

fs::copy_options FileHelper::create_options(bool overwrite, bool recur)
{
	fs::copy_options options{};

	if (overwrite)
	{
		options |= fs::copy_options::overwrite_existing;
	}

	if (recur)
	{
		options |= fs::copy_options::recursive;
	}

	return options;
}

uint64_t FileHelper::file_size()
{
	std::error_code ec;

	if (!fs::is_regular_file(m_path, ec))
		return {};

	return fs::file_size(m_path, ec);
}

uint64_t FileHelper::last_modified()
{
	std::error_code ec;
	const auto last = fs::last_write_time(m_path, ec);

	if (ec.value() != 0)
		return {};

	const auto windows_time = static_cast<uint64_t>(last.time_since_epoch().count());
	return pfc::fileTimeWtoU(windows_time);
}
