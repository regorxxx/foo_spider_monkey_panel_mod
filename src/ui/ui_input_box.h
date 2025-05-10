#pragma once

#include <resources/resource.h>

namespace smp::ui
{

class CInputBox
    : public CDialogImpl<CInputBox>
    , public CDialogResize<CInputBox>
{
public:
    CInputBox( std::string_view prompt, std::string_view caption, std::string_view value = "" );

    BEGIN_DLGRESIZE_MAP( CInputBox )
        DLGRESIZE_CONTROL( IDC_INPUT_PROMPT, DLSZ_SIZE_X | DLSZ_SIZE_Y )
        DLGRESIZE_CONTROL( IDC_INPUT_VALUE, DLSZ_SIZE_X | DLSZ_MOVE_Y )
        DLGRESIZE_CONTROL( IDOK, DLSZ_MOVE_X | DLSZ_MOVE_Y )
        DLGRESIZE_CONTROL( IDCANCEL, DLSZ_MOVE_X | DLSZ_MOVE_Y )
    END_DLGRESIZE_MAP()

    BEGIN_MSG_MAP( CInputBox )
        MSG_WM_INITDIALOG( OnInitDialog )
        MSG_WM_COMMAND( OnCommand )
        CHAIN_MSG_MAP( CDialogResize<CInputBox> )
    END_MSG_MAP()

    enum
    {
        IDD = IDD_DIALOG_INPUT
    };

    LRESULT OnInitDialog( HWND hwndFocus, LPARAM lParam );
    LRESULT OnCommand( UINT codeNotify, int id, HWND hwndCtl );
    std::string GetValue();

private:
    void AdjustPromptControlToFit();

private:
    std::string prompt_;
    std::string caption_;
    std::string value_;
};

} // namespace smp::ui
