#include <stdafx.h>

#include "js_panel_window_cui.h"

#include <com_objects/drop_target_impl.h>
#include <events/event_dispatcher.h>
#include <events/event_js_callback.h>
#include <utils/colour_helpers.h>

namespace
{
uie::window_factory<smp::panel::js_panel_window_cui> g_js_panel_window_cui;
} // namespace

namespace smp::panel
{

js_panel_window_cui::js_panel_window_cui()
    : js_panel_window(PanelType::CUI)
{
}

DWORD js_panel_window_cui::GetColour(unsigned type, const GUID& guid)
{
    COLORREF colour = 0; ///< black
    if (type <= cui::colours::colour_active_item_frame)
    {
        cui::colours::helper helper(guid);
        colour = helper.get_colour(static_cast<cui::colours::colour_identifier_t>(type));
    }

    return smp::colour::ColorrefToArgb(colour);
}

HFONT js_panel_window_cui::GetFont(unsigned type, const GUID& guid)
{
    try
    {
        auto api = static_api_ptr_t<cui::fonts::manager>();
        if (guid != pfc::guid_null)
        {
            return api->get_font(guid);
        }
        else if (type <= cui::fonts::font_type_labels)
        {
            return api->get_font(static_cast<cui::fonts::font_type_t>(type));
        }
    }
    catch (const exception_service_extension_not_found&)
    {
    }

    return nullptr;
}

LRESULT js_panel_window_cui::on_message(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_SYSKEYDOWN:
    case WM_KEYDOWN:
    {
        if (uie::window::g_process_keydown_keyboard_shortcuts(wp))
            return 0;
        break;
    }
    case static_cast<UINT>(smp::MiscMessage::size_limit_changed):
    {
        notify_size_limit_changed(wp);
        return 0;
    }
    default:
        break;
    }

    return OnMessage(hwnd, msg, wp, lp);
}

bool js_panel_window_cui::have_config_popup() const
{
    return true;
}

bool js_panel_window_cui::is_available(const uie::window_host_ptr&) const
{
    return true;
}

bool js_panel_window_cui::show_config_popup(HWND parent)
{
    ShowConfigure(parent);
    return true;
}

const GUID& js_panel_window_cui::get_extension_guid() const
{
    return smp::guid::window_cui;
}

unsigned js_panel_window_cui::get_type() const
{
    return uie::type_toolbar | uie::type_panel;
}

void js_panel_window_cui::get_category(pfc::string_base& out) const
{
    out = "Panels";
}

void js_panel_window_cui::get_config(stream_writer* writer, abort_callback& abort) const
{
    SaveSettings(*writer, abort);
}

void js_panel_window_cui::get_name(pfc::string_base& out) const
{
    out = SMP_NAME;
}

void js_panel_window_cui::set_config(stream_reader* reader, t_size size, abort_callback& abort)
{
    LoadSettings(*reader, size, abort, false);
}

uie::container_window_v3_config js_panel_window_cui::get_window_config()
{
    return { TEXT(SMP_WINDOW_CLASS_NAME), false, CS_DBLCLKS };
}

void js_panel_window_cui::notify_size_limit_changed(LPARAM lp)
{
    get_host()->on_size_limit_change(GetHWND(), lp);
}

} // namespace smp::panel
