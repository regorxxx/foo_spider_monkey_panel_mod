#pragma once

class AlbumArtStatic
{
public:
	using Type = GUID;

	static HRESULT to_istream(const album_art_data_ptr& data, wil::com_ptr<IStream>& stream);
	static Type get_type(size_t type_id);
	static album_art_data_ptr get(const metadb_handle_ptr& handle, size_t type_id, bool want_stub, bool only_embed, std::string& path);
	static album_art_data_ptr get_embedded(std::string_view path, size_t type_id);
	static album_art_data_ptr to_data(IStream* stream);
	static album_art_data_ptr to_data(std::wstring_view path);
	static bool check_type_id(size_t type_id);
	static std::unique_ptr<Gdiplus::Bitmap> to_bitmap(const album_art_data_ptr& data);
	static void show_viewer(const album_art_data_ptr& data);

private:
	static constexpr std::array types =
	{
		&album_art_ids::cover_front,
		&album_art_ids::cover_back,
		&album_art_ids::disc,
		&album_art_ids::icon,
		&album_art_ids::artist,
	};
};
