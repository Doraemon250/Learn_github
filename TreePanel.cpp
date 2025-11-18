#include "TreePanel.h"

//#include <wx/artprov.h>
//#include <wx/dcbuffer.h>
//#include <wx/dataobj.h>
//#include "CanvasPanel.h"


void TreePanel::InitTree()
{

    wxImageList* imageList = new wxImageList(16, 16);
    wxBitmap bitmapFile;
    bitmapFile.LoadFile("resource/image/OIP-C.png", wxBITMAP_TYPE_PNG);
    wxImage imageForDrawText = bitmapFile.ConvertToImage();
    bitmapFile = imageForDrawText.Scale(24, 24, wxIMAGE_QUALITY_HIGH);
    bitmapFile = wxBitmap(bitmapFile);
    imageList->Add(bitmapFile);
    m_tree->AssignImageList(imageList);
    wxTreeItemId root = m_tree->AddRoot("元件库");

    wxTreeItemId basic = m_tree->AppendItem(root, "基本逻辑门",0,0);
    m_tree->AppendItem(basic, "与门");
    m_tree->AppendItem(basic, "或门");
    m_tree->AppendItem(basic, "非门");

    wxTreeItemId basic2 = m_tree->AppendItem(root, "复合逻辑门", 0, 0);
    m_tree->AppendItem(basic2, "与非门");
    m_tree->AppendItem(basic2, "或非门");
    m_tree->AppendItem(basic2, "同或门");
    m_tree->AppendItem(basic2, "异或门");


    m_tree->Expand(root);
    m_tree->Expand(basic);
    m_tree->Expand(basic2);
}

TreePanel::TreePanel(wxPanel* parent)
    : wxPanel(parent, wxID_ANY)
{
    wxBoxSizer* s = new wxBoxSizer(wxVERTICAL);

    m_tree = new wxTreeCtrl(this, wxID_ANY);
    InitTree();
    s->Add(m_tree, 1, wxEXPAND);
    SetSizer(s);
}

wxBEGIN_EVENT_TABLE(TreePanel, wxPanel)
EVT_TREE_BEGIN_DRAG(wxID_ANY, TreePanel::OnBeginDrag)
wxEND_EVENT_TABLE()

void TreePanel::OnBeginDrag(wxTreeEvent& evt)
{
    wxTreeItemId id = evt.GetItem();
    if (!id.IsOk()) return;
    wxString name = m_tree->GetItemText(id);
    if (name == "基本门" || name == "元件库") return;   // 只让叶子可拖

    wxTextDataObject dragData(name);
    wxDropSource source(dragData, this);
    source.DoDragDrop(wxDrag_CopyOnly);
}

