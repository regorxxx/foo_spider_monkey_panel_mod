#include "ui_extension.h"

namespace win32_helpers {

void send_message_to_direct_children(HWND wnd_parent, UINT msg, WPARAM wp, LPARAM lp)
{
    HWND wnd = GetWindow(wnd_parent, GW_CHILD);
    if (wnd)
        do {
            SendMessage(wnd, msg, wp, lp);
        } while ((wnd = GetWindow(wnd, GW_HWNDNEXT)));
}

int message_box(HWND wnd, const TCHAR* text, const TCHAR* caption, UINT type)
{
    modal_dialog_scope scope(wnd);
    return MessageBox(wnd, text, caption, type);
}

}; // namespace win32_helpers

bool uRebar_InsertItem(HWND wnd, int n, uREBARBANDINFO* rbbi, bool insert)
{
    pfc::stringcvt::string_wide_from_utf8 text_utf16;

    REBARBANDINFOW rbw{};
    rbw.cbSize = REBARBANDINFOW_V6_SIZE;

    rbw.fMask = rbbi->fMask;
    rbw.fStyle = rbbi->fStyle;
    rbw.clrFore = rbbi->clrFore;
    rbw.clrBack = rbbi->clrBack;

    if (rbbi->fMask & RBBIM_TEXT) {
        text_utf16.convert(rbbi->lpText);
        rbw.lpText = const_cast<WCHAR*>(text_utf16.get_ptr());
    }

    rbw.iImage = rbbi->iImage;
    rbw.hwndChild = rbbi->hwndChild;
    rbw.cxMinChild = rbbi->cxMinChild;
    rbw.cyMinChild = rbbi->cyMinChild;
    rbw.cx = rbbi->cx;
    rbw.hbmBack = rbbi->hbmBack;
    rbw.wID = rbbi->wID;
#if (_WIN32_IE >= 0x0400) // we should check size of structure instead so keeping compatibility is possible, but
                          // whatever
    rbw.cyChild = rbbi->cyChild;
    rbw.cyMaxChild = rbbi->cyMaxChild;
    rbw.cyIntegral = rbbi->cyIntegral;
    rbw.cxIdeal = rbbi->cxIdeal;
    rbw.lParam = rbbi->lParam;
    rbw.cxHeader = rbbi->cxHeader;
#endif

    return SendMessage(wnd, insert ? RB_INSERTBANDW : RB_SETBANDINFOW, n, reinterpret_cast<LPARAM>(&rbw)) != 0;
}

int uTabCtrl_InsertItemText(HWND wnd, int idx, const char* text, bool insert)
{
    pfc::string8 temp2;
    uTCITEM tabs;
    memset(&tabs, 0, sizeof(uTCITEM));
    tabs.mask = TCIF_TEXT;
    uFixAmpersandChars_v2(text, temp2);
    tabs.pszText = const_cast<char*>(temp2.get_ptr());
    return insert ? uTabCtrl_InsertItem(wnd, idx, &tabs) : uTabCtrl_SetItem(wnd, idx, &tabs);
}

bool uComboBox_GetText(HWND combo, UINT index, pfc::string_base& out)
{
    int len = ComboBox_GetLBTextLen(combo, index);

    if (len < 0 || len > 16 * 1024 * 1024)
        return false;

    if (len == 0) {
        out.reset();
        return true;
    }

    pfc::array_t<WCHAR> temp;
    temp.set_size(len + 1);
    if (temp.get_ptr() == 0)
        return false;

    temp.fill(0);
    len = ComboBox_GetLBText(combo, index, temp.get_ptr());

    if (len == CB_ERR)
        return false;

    out.set_string(pfc::stringcvt::string_utf8_from_wide(temp.get_ptr()));
    return true;
}

bool uStatus_SetText(HWND wnd, int part, const char* text)
{
    const pfc::stringcvt::string_wide_from_utf8 text_utf16(text);
    return SendMessage(wnd, SB_SETTEXTW, part, (LPARAM)(text_utf16.get_ptr())) != 0;
}

HFONT uCreateIconFont()
{
    LOGFONTW lf;
    memset(&lf, 0, sizeof(LOGFONTW));
    SystemParametersInfoW(SPI_GETICONTITLELOGFONT, 0, &lf, 0);

    return CreateFontIndirectW(&lf);
}

HFONT uCreateMenuFont(bool vertical)
{
    NONCLIENTMETRICSW ncm;
    memset(&ncm, 0, sizeof(NONCLIENTMETRICSW));
    ncm.cbSize = sizeof(NONCLIENTMETRICSW);
    SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);

    if (vertical) {
        ncm.lfMenuFont.lfEscapement = 900;
        ncm.lfMenuFont.lfOrientation = 900;
    }

    return CreateFontIndirectW(&ncm.lfMenuFont);
}

void uGetMenuFont(LOGFONT* p_lf)
{
    NONCLIENTMETRICS ncm;
    memset(&ncm, 0, sizeof(NONCLIENTMETRICS));
    ncm.cbSize = sizeof(NONCLIENTMETRICS);
    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);

    *p_lf = ncm.lfMenuFont;
}

void uGetIconFont(LOGFONT* p_lf)
{
    SystemParametersInfo(SPI_GETICONTITLELOGFONT, 0, p_lf, 0);
}

BOOL uFormatMessage(DWORD dw_error, pfc::string_base& out)
{
    BOOL rv = FALSE;
    pfc::array_t<WCHAR> buffer;
    constexpr unsigned buffer_size = 256;
    buffer.set_size(buffer_size);
    // MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    if (FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 0, dw_error, 0, buffer.get_ptr(),
            buffer_size, 0)) {
        out.set_string(pfc::stringcvt::string_utf8_from_wide(buffer.get_ptr(), buffer.get_size()));
        rv = TRUE;
    }
    return rv;
}

#if (WINVER >= 0x0500)
HWND uRecursiveChildWindowFromPoint(HWND parent, POINT pt_parent)
{
    HWND wnd = RealChildWindowFromPoint(parent, pt_parent);
    if (wnd && wnd != parent) {
        HWND wnd_last = wnd;
        POINT pt = pt_parent;
        MapWindowPoints(parent, wnd_last, &pt, 1);
        for (;;) {
            wnd = RealChildWindowFromPoint(wnd_last, pt);
            if (!wnd)
                return 0;
            if (wnd == wnd_last)
                return wnd;
            MapWindowPoints(wnd_last, wnd, &pt, 1);
            wnd_last = wnd;
            RECT rc;
            GetClientRect(wnd_last, &rc);
            if (!PtInRect(&rc, pt))
                return wnd_last;
        }
    }
    return 0;
}

BOOL uGetMonitorInfo(HMONITOR monitor, LPMONITORINFO lpmi)
{
    BOOL rv = FALSE;
    if (lpmi->cbSize == sizeof(uMONITORINFOEX)) {
        uLPMONITORINFOEX lpmiex = (uLPMONITORINFOEX)lpmi;

        MONITORINFOEXW mi;
        memset(&mi, 0, sizeof(MONITORINFOEXW));
        mi.cbSize = sizeof(MONITORINFOEXW);

        rv = GetMonitorInfoW(monitor, &mi);

        lpmi->rcMonitor = mi.rcMonitor;
        lpmi->rcWork = mi.rcWork;
        lpmi->dwFlags = mi.dwFlags;

        pfc::stringcvt::string_utf8_from_wide temp(mi.szDevice, CCHDEVICENAME);
        strcpy_utf8_truncate(temp, lpmiex->szDevice, CCHDEVICENAME);
    } else
        rv = GetMonitorInfoW(monitor, lpmi);
    return rv;
}

namespace uie::win32 {

LRESULT paint_background_using_parent(HWND wnd, HDC dc, bool use_wm_printclient)
{
    const auto wnd_parent = GetParent(wnd);
    auto top_left = POINT{0, 0};
    auto previous_origin = POINT{};

    MapWindowPoints(wnd, wnd_parent, &top_left, 1);
    OffsetWindowOrgEx(dc, top_left.x, top_left.y, &previous_origin);
    auto _ = fb2k::callOnRelease(
        [dc, previous_origin] { SetWindowOrgEx(dc, previous_origin.x, previous_origin.y, nullptr); });

    if (use_wm_printclient)
        return SendMessage(wnd_parent, WM_PRINTCLIENT, reinterpret_cast<WPARAM>(dc), PRF_ERASEBKGND);

    return SendMessage(wnd_parent, WM_ERASEBKGND, reinterpret_cast<WPARAM>(dc), 0);
}

} // namespace uie::win32

#endif
