#include <stdafx.h>

#include "ui_editor_config.h"

#include <2K3/FileDialog.hpp>
#include <ui/scintilla/sci_config.h>
#include <ui/ui_name_value_edit.h>

namespace fs = std::filesystem;

namespace smp::ui
{

CDialogEditorConfig::CDialogEditorConfig()
{
}

BOOL CDialogEditorConfig::OnInitDialog(HWND, LPARAM)
{
    DoDataExchange();

    SetWindowTheme(propertiesListView_.m_hWnd, L"explorer", nullptr);

    propertiesListView_.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);
    propertiesListView_.AddColumn(L"Name", 0);
    propertiesListView_.SetColumnWidth(0, 150);
    propertiesListView_.AddColumn(L"Value", 1);
    propertiesListView_.SetColumnWidth(1, 310);
    LoadProps();

    return TRUE; // set focus to default control
}

LRESULT CDialogEditorConfig::OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
    switch (wID)
    {
    case IDOK:
    {
        EndDialog(IDOK);
        break;
    }
    case IDCANCEL:
    {
        EndDialog(IDCANCEL);
        break;
    }
    default:
    {
        assert(0);
    }
    }

    return 0;
}

void CDialogEditorConfig::OnButtonReset(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
    LoadProps(true);
}

void CDialogEditorConfig::OnButtonExportBnClicked(WORD, WORD, HWND)
{
    auto path_func = [this](fb2k::stringRef path)
        {
            const auto native = filesystem::g_get_native_path(path->c_str());
            const auto wpath = qwr::unicode::ToWide(native);
            config::sci::props.export_to_file(wpath);
        };

    FileDialog::save(m_hWnd, "Save as", "Configuration files|*.cfg|All files|*.*", "cfg", path_func);
}

void CDialogEditorConfig::OnButtonImportBnClicked(WORD, WORD, HWND)
{
    auto path_func = [this](fb2k::stringRef path)
        {
            const auto native = filesystem::g_get_native_path(path->c_str());
            const auto wpath = qwr::unicode::ToWide(native);
            config::sci::props.import_from_file(wpath);
            LoadProps();
        };

    FileDialog::open(m_hWnd, "Import from", "Configuration files|*.cfg|All files|*.*", path_func);
}

LRESULT CDialogEditorConfig::OnPropNMDblClk(LPNMHDR pnmh)
{
    auto pniv = reinterpret_cast<LPNMITEMACTIVATE>(pnmh);

    if (pniv->iItem < 0)
    {
        return 0;
    }

    const auto key = this->GetItemTextStr(pniv->iItem, 0);
    const auto val = this->GetItemTextStr(pniv->iItem, 1);

    CNameValueEdit dlg(key.c_str(), val.c_str(), "Edit property");
    if (IDOK == dlg.DoModal(m_hWnd))
    {
        const auto newVal = dlg.GetValue();

        // Save
        auto& prop_sets = config::sci::props.val();
        auto it = ranges::find_if(prop_sets, [&key](const auto& elem) { return (elem.key == key); });
        if (it != prop_sets.end())
        {
            it->val = newVal;
        }

        // Update list
        propertiesListView_.SetItemText(pniv->iItem, 1, qwr::unicode::ToWide(newVal).c_str());
        DoDataExchange();
    }

    return 0;
}

void CDialogEditorConfig::LoadProps(bool reset)
{
    if (reset)
    {
        config::sci::props.reset();
    }

    const auto& prop_sets = config::sci::props.val();

    propertiesListView_.DeleteAllItems();

    for (auto&& [i, prop]: ranges::views::enumerate(prop_sets))
    {
        propertiesListView_.AddItem(i, 0, qwr::unicode::ToWide(prop.key).c_str());
        propertiesListView_.AddItem(i, 1, qwr::unicode::ToWide(prop.val).c_str());
    }
}

std::string CDialogEditorConfig::GetItemTextStr(int nItem, int nSubItem)
{
    constexpr size_t kBufferLen = 256;
    std::wstring buffer;
    buffer.resize(kBufferLen);

    auto size = propertiesListView_.GetItemText(nItem, nSubItem, buffer.data(), buffer.size());
    // size == wcslen(buffer.c_str())
    buffer.resize(size);

    return qwr::unicode::ToU8(buffer);
}

} // namespace smp::ui
