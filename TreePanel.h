#pragma once
#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/dnd.h>          // ÍÏ×§Ö§³Ö
//
//void OnBeginDrag(wxTreeEvent& evt);

class TreePanel : public wxPanel
{
public:
    TreePanel(wxPanel* parent);
private:
    void InitTree();
    void OnBeginDrag(wxTreeEvent& evt);
    wxTreeCtrl* m_tree;

    wxDECLARE_EVENT_TABLE();
};
