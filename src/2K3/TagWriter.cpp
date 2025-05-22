#include <stdafx.h>
#include "TagWriter.hpp"
#include "FileInfoFilter.hpp"

#include <foobar2000/SDK/file_info_filter_impl.h>

TagWriter::TagWriter(metadb_handle_list_cref handles) : m_handles(handles) {}

std::string TagWriter::json_to_string(JSON& j)
{
	if (j.is_string())
	{
		const auto str = j.get<std::string>();
		return pfc::string8(str.c_str()).trim(' ').get_ptr();
	}
	else if (j.is_number())
	{
		return j.dump();
	}
	else
	{
		return {};
	}
}

std::vector<std::string> TagWriter::json_to_strings(JSON& j)
{
	if (!j.is_array())
	{
		j = JSON::array({ j });
	}

	auto transform = [](auto&& j2) { return json_to_string(j2); };
	auto filter = [](auto&& str) { return str.length() > 0; };
	return j | ranges::views::transform(transform) | ranges::views::filter(filter) | ranges::to<std::vector<std::string>>();
}

void TagWriter::from_json_array(JSON& arr)
{
	std::vector<file_info_impl> infos(m_handles.get_count());

	for (auto&& [info, obj, handle] : ranges::views::zip(infos, arr, m_handles))
	{
		qwr::QwrException::ExpectTrue(obj.is_object() && !obj.empty(), "Invalid JSON info: array element not a JSON object");

		info = handle->get_info_ref()->info();

		for (const auto& [name, values] : obj.items())
		{
			qwr::QwrException::ExpectTrue(!name.empty(), "Invalid JSON info: name cannot be empty");

			info.meta_remove_field(name.c_str());

			for (auto&& value : json_to_strings(values))
			{
				info.meta_add(name.c_str(), value.c_str());
			}
		}
	}

	auto list = pfc::ptr_list_const_array_t<const file_info, file_info_impl*>(infos.data(), infos.size());
	auto filter = fb2k::service_new<file_info_filter_impl>(m_handles, list);
	metadb_io_v2::get()->update_info_async(m_handles, filter, core_api::get_main_window(), metadb_io_v2::op_flag_silent, nullptr);
}

void TagWriter::from_json_object(JSON& obj)
{
	FileInfoFilter::Tags tags;

	for (const auto& [name, value] : obj.items())
	{
		qwr::QwrException::ExpectTrue(!name.empty(), "Invalid JSON info: name cannot be empty");

		const auto tag = FileInfoFilter::Tag{
			.name = name,
			.values = json_to_strings(value)
		};

		tags.emplace_back(tag);
	}

	auto filter = fb2k::service_new<FileInfoFilter>(tags);
	metadb_io_v2::get()->update_info_async(m_handles, filter, core_api::get_main_window(), metadb_io_v2::op_flag_silent, nullptr);
}
