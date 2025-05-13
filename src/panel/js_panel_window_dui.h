#pragma once

#include <panel/js_panel_window.h>

namespace smp::panel
{

class js_panel_window_dui: public js_panel_window, public ui_element_instance, public CWindowImpl<js_panel_window_dui>
{
public:
    js_panel_window_dui(ui_element_config::ptr cfg, ui_element_instance_callback::ptr callback);

    static GUID g_get_guid();
    static GUID g_get_subclass();
    static pfc::string8 g_get_description();
    static ui_element_config::ptr g_get_default_configuration();
    static void g_get_name(pfc::string_base& out);

    // js_panel_window
    DWORD GetColour(unsigned type, const GUID& guid) override;
    HFONT GetFont(unsigned type, const GUID& guid) override;

    // ui_element_instance
    GUID get_guid() override;
    GUID get_subclass() override;
    bool edit_mode_context_menu_test(const POINT& p_point, bool p_fromkeyboard) override;
    ui_element_config::ptr get_configuration() override;
    void edit_mode_context_menu_build(const POINT& p_point, bool p_fromkeyboard, HMENU p_menu, unsigned p_id_base) override;
    void edit_mode_context_menu_command(const POINT& p_point, bool p_fromkeyboard, unsigned p_id, unsigned p_id_base) override;
    void set_configuration(ui_element_config::ptr data) override;

    BOOL ProcessWindowMessage(HWND wnd, uint32_t msg, WPARAM wp, LPARAM lp, LRESULT& lres, DWORD) override;

    void initialize_window(HWND parent);

private:
    // js_panel_window
    void notify_size_limit_changed(LPARAM lp) override;

private:
    ui_element_instance_callback::ptr uiCallback_;
};

} // namespace smp::panel
