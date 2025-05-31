#include <stdafx.h>
#include "error_popup.h"

namespace qwr
{

void ReportErrorWithPopup(const std::string& title, const std::string& errorText)
{
    fb2k::inMainThread([title, errorText]
        {
            FB2K_console_formatter() << title.c_str() << ":\n" << errorText.c_str() << "\n";
            popup_message::g_show(errorText.c_str(), title.c_str());

            MessageBeep(MB_ICONASTERISK);
        });
}

} // namespace qwr
