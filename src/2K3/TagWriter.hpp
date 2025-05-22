#pragma once

class TagWriter
{
public:
	TagWriter(metadb_handle_list_cref handles);

	void from_json_array(JSON& arr);
	void from_json_object(JSON& obj);

private:
	static std::string json_to_string(JSON& j);
	static std::vector<std::string> json_to_strings(JSON& j);

	metadb_handle_list m_handles;
};
