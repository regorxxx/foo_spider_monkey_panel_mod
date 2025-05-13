#pragma once

#include <panel/js_panel_window.h>

namespace smp::panel
{

class js_panel_window_cui : public js_panel_window, public uie::container_uie_window_v3
{
public:
    js_panel_window_cui();

protected:
    // js_panel_window
    DWORD GetColour(unsigned type, const GUID& guid) override;
    HFONT GetFont(unsigned type, const GUID& guid) override;

    // uie::window
    [[nodiscard]] bool have_config_popup() const override;
    [[nodiscard]] bool is_available(const uie::window_host_ptr& p) const override;
    bool show_config_popup(HWND parent) override;
    [[nodiscard]] const GUID& get_extension_guid() const override;
    [[nodiscard]] unsigned get_type() const override;
    void get_category(pfc::string_base& out) const override;
    void get_config(stream_writer* writer, abort_callback& abort) const override;
    void get_name(pfc::string_base& out) const override;
    void set_config(stream_reader* reader, t_size size, abort_callback& abort) override;
    LRESULT on_message(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) override;
    uie::container_window_v3_config get_window_config() override;

private:
    // js_panel_window
    void notify_size_limit_changed(LPARAM lp) override;
};

} // namespace smp::panel
