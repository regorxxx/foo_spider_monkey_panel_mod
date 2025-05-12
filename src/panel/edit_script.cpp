#include <stdafx.h>

#include "edit_script.h"

#include <config/parsed_panel_config.h>
#include <utils/edit_text.h>

#include <qwr/error_popup.h>
#include <qwr/winapi_error_helpers.h>

namespace fs = std::filesystem;

namespace smp::panel
{

void EditScript( HWND hParent, config::ParsedPanelSettings& settings )
{
    try
    {
        switch ( settings.GetSourceType() )
        {
        case config::ScriptSourceType::Sample:
        {
            const int iRet = popup_message_v3::get()->messageBox(
                hParent,
                "Are you sure?\n\n"
                "You are trying to edit a sample script.\n"
                "Any changes performed to the script will be applied to every panel that are using this sample.\n"
                "These changes will also be lost when updating the component.",
                "Editing script",
                MB_YESNO | MB_ICONWARNING );

            if ( iRet != IDYES )
            {
                break;
            }

            assert( settings.scriptPath );
            const auto filePath = *settings.scriptPath;
            qwr::QwrException::ExpectTrue( fs::exists( filePath ), "Sample script is missing: {}", filePath.u8string() );

            smp::EditTextFile( hParent, filePath, true, true );
            break;
        }
        case config::ScriptSourceType::File:
        {
            assert( settings.scriptPath );
            const auto filePath = *settings.scriptPath;
            qwr::QwrException::ExpectTrue( fs::exists( filePath ), "Script is missing: {}", filePath.u8string() );

            smp::EditTextFile( hParent, filePath, true, true );
            break;
        }
        case config::ScriptSourceType::InMemory:
        {
            assert( settings.script );
            smp::EditText( hParent, *settings.script, true );
            break;
        }
        case config::ScriptSourceType::Package:
        default:
        {
            assert( false );
            break;
        }
        }
    }
    catch ( const fs::filesystem_error& e )
    {
        throw qwr::QwrException( e );
    }
}

void EditPackageScript( HWND hParent, const std::filesystem::path& script, const config::ParsedPanelSettings& settings )
{
    try
    {
        assert( settings.GetSourceType() == config::ScriptSourceType::Package );
        if ( settings.isSample )
        {
            const int iRet = popup_message_v3::get()->messageBox(
                hParent,
                "Are you sure?\n\n"
                "You are trying to edit a sample script.\n"
                "Any changes performed to the script will be applied to every panel that are using this sample.\n"
                "These changes will also be lost when updating the component.",
                "Editing script",
                MB_YESNO | MB_ICONWARNING );

            if ( iRet != IDYES )
            {
                return;
            }
        }

        qwr::QwrException::ExpectTrue( fs::exists( script ), "Script is missing: {}", script.u8string() );

        smp::EditTextFile( hParent, script, true, true );
    }
    catch ( const fs::filesystem_error& e )
    {
        throw qwr::QwrException( e );
    }
}

} // namespace smp::panel
