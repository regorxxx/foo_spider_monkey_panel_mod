#pragma once

#include <resources/resource.h>

namespace smp::ui
{

class CNameValueEdit : public CDialogImpl<CNameValueEdit>
{
public:
    BEGIN_MSG_MAP( CNameValueEdit )
        MSG_WM_INITDIALOG( OnInitDialog )
        MSG_WM_COMMAND( OnCommand )
    END_MSG_MAP()

    enum
    {
        IDD = IDD_DIALOG_NAME_VALUE
    };

    CNameValueEdit( const char* name, const char* value, const char* caption );
    std::string GetValue();

private:
    LRESULT OnInitDialog( HWND hwndFocus, LPARAM lParam );
    LRESULT OnCommand( UINT codeNotify, int id, HWND hwndCtl );

private:
    std::string name_;
    std::string value_;
    std::string caption_;
};

} // namespace smp::ui
