#include <stdafx.h>

#include "js_panel_window_dui.h"

#include <com_objects/drop_target_impl.h>
#include <events/event_dispatcher.h>
#include <events/event_js_callback.h>
#include <utils/colour_helpers.h>

#include <foobar2000/helpers/atl-misc.h>

namespace smp::panel
{

js_panel_window_dui::js_panel_window_dui( ui_element_config::ptr cfg, ui_element_instance_callback::ptr callback )
    : js_panel_window( PanelType::DUI )
    , uiCallback_( callback )
{
    set_configuration( cfg );
}

GUID js_panel_window_dui::g_get_guid()
{
    return smp::guid::window_dui;
}

GUID js_panel_window_dui::g_get_subclass()
{
    return ui_element_subclass_utility;
}

pfc::string8 js_panel_window_dui::g_get_description()
{
    return "Customizable panel with JavaScript support.";
}

ui_element_config::ptr js_panel_window_dui::g_get_default_configuration()
{
    ui_element_config_builder builder;
    config::PanelSettings::SaveDefault( builder.m_stream, fb2k::noAbort );
    return builder.finish( g_get_guid() );
}

void js_panel_window_dui::g_get_name( pfc::string_base& out )
{
    out = SMP_NAME;
}

GUID js_panel_window_dui::get_guid()
{
    return g_get_guid();
}

GUID js_panel_window_dui::get_subclass()
{
    return g_get_subclass();
}

DWORD js_panel_window_dui::GetColour( unsigned type, const GUID& guid )
{
    const auto& guidToQuery = [type, &guid] {
        // Take care when changing this array:
        // guid indexes are part of SMP API
        const std::array<const GUID*, 4> guids = {
            &ui_color_text,
            &ui_color_background,
            &ui_color_highlight,
            &ui_color_selection,
        };

        if ( guid != pfc::guid_null )
        {
            return guid;
        }
        else if ( type < guids.size() )
        {
            return *guids[type];
        }
        else
        {
            return pfc::guid_null;
        }
    }();

    t_ui_color colour = 0; ///< black
    if ( guidToQuery != pfc::guid_null )
    {
        colour = uiCallback_->query_std_color( guidToQuery );
    }

    return smp::colour::ColorrefToArgb( colour );
}

HFONT js_panel_window_dui::GetFont( unsigned type, const GUID& guid )
{
    const auto& guidToQuery = [type, &guid] {
        // Take care when changing this array:
        // guid indexes are part of SMP API
        const std::array<const GUID*, 6> guids = {
            &ui_font_default,
            &ui_font_tabs,
            &ui_font_lists,
            &ui_font_playlists,
            &ui_font_statusbar,
            &ui_font_console,
        };

        if ( guid != pfc::guid_null )
        {
            return guid;
        }
        else if ( type < guids.size() )
        {
            return *guids[type];
        }
        else
        {
            return pfc::guid_null;
        }
    }();

    return ( guidToQuery != pfc::guid_null ? uiCallback_->query_font_ex( guidToQuery ) : nullptr );
}

BOOL js_panel_window_dui::ProcessWindowMessage(HWND wnd, uint32_t msg, WPARAM wp, LPARAM lp, LRESULT& lres, DWORD)
{
    switch (msg)
    {
    case WM_RBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONDBLCLK:
    case WM_CONTEXTMENU:
    {
        if (uiCallback_->is_edit_mode_enabled())
        {
            return FALSE;
        }
        break;
    }
    case static_cast<UINT>(smp::MiscMessage::size_limit_changed):
    {
        notify_size_limit_changed(wp);
        return FALSE;
    }
    }

    lres = OnMessage(wnd, msg, wp, lp);
    return TRUE;
}

bool js_panel_window_dui::edit_mode_context_menu_test( const POINT&, bool )
{
    return true;
}

ui_element_config::ptr js_panel_window_dui::get_configuration()
{
    ui_element_config_builder builder;
    SaveSettings( builder.m_stream, fb2k::noAbort );
    return builder.finish( g_get_guid() );
}

void js_panel_window_dui::edit_mode_context_menu_build( const POINT& p_point, bool, HMENU p_menu, unsigned p_id_base )
{
    GenerateContextMenu( p_menu, p_point.x, p_point.y, p_id_base );
}

void js_panel_window_dui::edit_mode_context_menu_command( const POINT&, bool, unsigned p_id, unsigned p_id_base )
{
    ExecuteContextMenu( p_id, p_id_base );
}

void js_panel_window_dui::set_configuration( ui_element_config::ptr data )
{
    ui_element_config_parser parser( data );

    // FIX: If window already created, DUI won't destroy it and create it again.
    LoadSettings( parser.m_stream, parser.get_remaining(), fb2k::noAbort, !!GetHWND() );
}

void js_panel_window_dui::initialize_window( HWND parent )
{
    Create(parent);
}

void js_panel_window_dui::notify_size_limit_changed( LPARAM )
{
    uiCallback_->on_min_max_info_change();
}


class impl : public ui_element_impl<js_panel_window_dui> {};

FB2K_SERVICE_FACTORY(impl);

} // namespace smp::panel
