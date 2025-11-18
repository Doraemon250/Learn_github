#include "cMain.h"
#include <wx/splitter.h>
#include <wx/panel.h>
#include<wx/notebook.h>
#include<wx/sizer.h>
#include <wx/treectrl.h>
#include"DrawBoard.h"
#include"TreePanel.h"
#include <wx/artprov.h>
#include <wx/filename.h>
#include "Netlist.h"

enum { 
	ID_idle=001,
	ID_drawline = 002,
	ID_drawtext=003,
	ID_deleteSelected=004,
	ID_zoomin=005,
	ID_zoomout=006
            };
cMain::cMain() : wxFrame(nullptr,wxID_ANY,"电路编辑器",wxPoint(30,30),wxSize(800,600))
{

	wxInitAllImageHandlers();

	wxBoxSizer* cMainSizer = new wxBoxSizer(wxVERTICAL);//cMain布局
	SetSizer(cMainSizer);

	//主菜单栏
	wxMenuBar* menuBar = new wxMenuBar;
	//文件菜单
	wxMenu* fileMenu = new wxMenu;
	fileMenu->Append(wxID_NEW, "&新建\tCtrl+N");
	fileMenu->Append(wxID_OPEN, "&打开...\tCtrl+M");
	fileMenu->Append(wxID_CLEAR, "&清除\tC");

	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_CLOSE, "&关闭\tCtrl+Shift+W");
	fileMenu->Append(wxID_SAVE, "&保存\tCtrl+S");
	fileMenu->Append(wxID_SAVEAS, "&另存为...\tCtrl+Shift+S");
	fileMenu->Append(wxID_NEW, "&导出项目\tCtrl+Shift+E");
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, "&退出\tCtrl+Q");
	menuBar->Append(fileMenu, "&文件");
	//编辑菜单
	wxMenu* editMenu = new wxMenu;

	menuBar->Append(editMenu, "&编辑");
	//项目菜单
	wxMenu* projectMenu = new wxMenu;

	menuBar->Append(projectMenu, "&项目");
	//模拟菜单
	wxMenu* simulateMenu = new wxMenu;

	menuBar->Append(simulateMenu, "&模拟");
	//窗口菜单
	wxMenu* windowMenu = new wxMenu;

	menuBar->Append(windowMenu, "&窗口");
	//帮助菜单
	wxMenu* helpMenu = new wxMenu;

	menuBar->Append(helpMenu, "&帮助");

	SetMenuBar(menuBar);

	//主面板
	wxPanel* m_mainPnl = new wxPanel(this, wxID_ANY);
	wxBoxSizer* m_mainPnlSizer = new wxBoxSizer(wxVERTICAL);//m_mainPnl布局
	m_mainPnl->SetSizer(m_mainPnlSizer);
	cMainSizer->Add(m_mainPnl, 1, wxEXPAND | wxALL, 0);

	//左右分割容器
	wxSplitterWindow* splitter1 = new wxSplitterWindow(m_mainPnl, wxID_ANY, wxDefaultPosition, wxDefaultSize,wxSP_LIVE_UPDATE| wxSP_BORDER);//
	m_mainPnlSizer->Add(splitter1, 1, wxEXPAND | wxALL, 0);
	splitter1->SetDoubleBuffered(true);
	

	//左面板
	
	wxPanel* m_leftPanel = new wxPanel(splitter1,wxID_ANY);
	wxBoxSizer* m_leftPanelSizer = new wxBoxSizer(wxVERTICAL);//m_leftPanel布局
	m_leftPanel ->SetSizer(m_leftPanelSizer);
	//上下分割器
	wxSplitterWindow* splitter2 = new wxSplitterWindow(m_leftPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE| wxSP_BORDER);//
	m_leftPanelSizer->Add(splitter2, 1, wxEXPAND | wxALL, 0);
	splitter2->SetDoubleBuffered(true);

	    //上面的notebook
	wxPanel* m_upPanel = new wxPanel(splitter2, wxID_ANY);
	wxBoxSizer* m_upPanelSizer = new wxBoxSizer(wxVERTICAL);//m_upPanel布局
	m_upPanel->SetSizer(m_upPanelSizer);
	wxNotebook* m_upNotebook = new wxNotebook(m_upPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);//
	m_upPanelSizer->Add(m_upNotebook, 1, wxEXPAND | wxALL, 0);
	wxPanel* m_panel1 = new wxPanel(m_upNotebook, wxID_ANY);
	wxPanel* m_panel2 = new wxPanel(m_upNotebook, wxID_ANY);
	m_panel1->SetBackgroundColour(wxColour(255, 255, 255));
	wxBoxSizer* m_panel1Sizer = new wxBoxSizer(wxVERTICAL);//m_panel1布局
	m_panel1->SetSizer(m_panel1Sizer);
	m_panel2->SetBackgroundColour(wxColour(255, 255, 255));
	//设计笔记本容器
	m_upNotebook->AddPage(m_panel1, "设计",true);
	        //左侧工具栏
	wxBitmap bitmapAdd;
	bitmapAdd.LoadFile("resource/image/780.png", wxBITMAP_TYPE_PNG);
	wxImage imageAdd = bitmapAdd.ConvertToImage();
	bitmapAdd = imageAdd.Scale(20, 20, wxIMAGE_QUALITY_HIGH);
	bitmapAdd = wxBitmap(bitmapAdd);
	wxToolBar* left_toolBar = new wxToolBar(m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL);
	left_toolBar->AddTool(201, wxT("添加线路"), bitmapAdd, wxT("添加线路"));
	left_toolBar->Realize();
	m_panel1Sizer->Add(left_toolBar, 0, wxEXPAND | wxALL, 0);
	        //树控件
	TreePanel* treeCtrl =new  TreePanel(m_panel1);
	m_panel1Sizer->Add(treeCtrl, 1, wxEXPAND | wxALL, 0);
	m_upNotebook->AddPage(m_panel2, "模拟");

	    //下面的notebook
	wxPanel* m_downPanel = new wxPanel(splitter2, wxID_ANY);
	wxBoxSizer* m_downPanelSizer = new wxBoxSizer(wxVERTICAL);//m_downPanel布局
	m_downPanel->SetSizer(m_downPanelSizer);
	wxNotebook* m_downNotebook = new wxNotebook(m_downPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);//
	m_downPanelSizer->Add(m_downNotebook, 1, wxEXPAND | wxALL, 0);
	wxPanel* m_panel3 = new wxPanel(m_downNotebook, wxID_ANY);
	wxPanel* m_panel4 = new wxPanel(m_downNotebook, wxID_ANY);
	m_panel3->SetBackgroundColour(wxColour(255, 255, 255));
	m_panel4->SetBackgroundColour(wxColour(255, 255, 255));
	m_downNotebook->AddPage(m_panel3, "属性");
	m_downNotebook->AddPage(m_panel4, "状态");
	//属性面板
	wxBoxSizer* m_panel3Sizer = new wxBoxSizer(wxVERTICAL);//m_panel3布局
	m_panel3->SetSizer(m_panel3Sizer);
	    //属性表控件



	    //放容器进割上下容器
	splitter2->SplitHorizontally(m_upPanel, m_downPanel, 300);
	//右面板

	wxPanel* m_rightPanel = new wxPanel(splitter1, wxID_ANY);
	wxBoxSizer* m_rightPanelSizer = new wxBoxSizer(wxVERTICAL);//m_rightPanel布局
	m_rightPanel->SetSizer(m_rightPanelSizer);
	//右侧工具栏
	wxBitmap bitmapForText;
	bitmapForText.LoadFile("resource/image/insertText.png", wxBITMAP_TYPE_PNG);
	wxImage imageForText = bitmapForText.ConvertToImage();
	bitmapForText = imageForText.Scale(24, 24, wxIMAGE_QUALITY_HIGH);
	bitmapForText = wxBitmap(bitmapForText);

	wxBitmap bitmapForIdle;
	bitmapForIdle.LoadFile("resource/image/select.png", wxBITMAP_TYPE_PNG);
	wxImage imageForIdle = bitmapForIdle.ConvertToImage();
	bitmapForIdle = imageForIdle.Scale(24, 24, wxIMAGE_QUALITY_HIGH);
	bitmapForIdle = wxBitmap(bitmapForIdle);

	wxBitmap bitmapForDrawLine;
	bitmapForDrawLine.LoadFile("resource/image/drawLine.png", wxBITMAP_TYPE_PNG);
	wxImage imageForDrawLine = bitmapForDrawLine.ConvertToImage();
	bitmapForDrawLine = imageForDrawLine.Scale(24, 24, wxIMAGE_QUALITY_HIGH);
	bitmapForDrawLine = wxBitmap(bitmapForDrawLine);

	wxToolBar* right_toolBar = new wxToolBar(m_rightPanel, wxID_ANY,wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL);
	right_toolBar->AddTool(ID_idle, wxT("选择"), bitmapForIdle, wxT("选择工具"));
	right_toolBar->AddTool(ID_drawline, wxT("连线"), bitmapForDrawLine, wxT("连线工具"));
	right_toolBar->AddTool(ID_drawtext, wxT("写文字"), bitmapForText, wxT("写文字"));
	right_toolBar->AddTool(ID_deleteSelected, "删除", wxArtProvider::GetBitmap(wxART_DELETE, wxART_TOOLBAR),wxT("删除"));
	right_toolBar->AddTool(ID_zoomin, "放大", wxArtProvider::GetBitmap(wxART_PLUS),wxT("放大"));
	right_toolBar->AddTool(ID_zoomout, "放小", wxArtProvider::GetBitmap(wxART_MINUS),wxT("缩小"));

	right_toolBar->Realize();

	drawborad = new DrawBoard(m_rightPanel);
	//放入右侧面板布局管理器
	m_rightPanelSizer->Add(right_toolBar, 0, wxEXPAND | wxALL, 5);
	m_rightPanelSizer->Add(drawborad, 1, wxEXPAND | wxALL, 5);

	//放面板进入左右分割容器
	splitter1->SplitVertically(m_leftPanel, m_rightPanel, 600);

	Bind(wxEVT_MENU, &cMain::OnSave, this, wxID_SAVE);
	Bind(wxEVT_MENU, &cMain::OnOpen, this, wxID_OPEN);
	Bind(wxEVT_MENU, &cMain::OnClear, this, wxID_CLEAR);

	Bind(wxEVT_MENU, &cMain::OnDrawIdle, this, ID_idle);
	Bind(wxEVT_MENU, &cMain::OnDrawLine,this, ID_drawline);
	Bind(wxEVT_MENU, &cMain::OnDrawText, this, ID_drawtext);
	Bind(wxEVT_MENU, &cMain::OnZoomIn, this, ID_zoomin);
	Bind(wxEVT_MENU, &cMain::OnZoomOut, this, ID_zoomout);
	Bind(wxEVT_MENU, &cMain::OnDeleteSelected, this, ID_deleteSelected);
	
}

cMain::~cMain() {
	
}

void cMain::OnDrawIdle(wxCommandEvent& event) {
	drawborad->SetCurrentTool(TOOL_IDEL);
}

void cMain::OnDrawLine(wxCommandEvent& event) {
	drawborad->SetCurrentTool(TOOL_WIRE);

}

void cMain::OnDrawText(wxCommandEvent& event) {
	drawborad->SetCurrentTool(TOOL_TEXT);
}

void cMain::OnSave(wxCommandEvent& event) {
	if (m_currentFilePath.IsEmpty()) {
		wxString filename = wxFileSelector("Save File", wxEmptyString, wxEmptyString, "json", "JSON files (*.json)|*.json", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		m_currentFilePath = filename;
	}

	if (!m_currentFilePath.empty()) {
		if (!drawborad) return;

		Netlist netlist = NetlistUtils::ExportFromCanvas(
			drawborad->m_components,
			drawborad->m_wires
		);

		if (netlist.circuitName.empty()) {
			wxFileName fn(m_currentFilePath);
			netlist.circuitName = fn.GetName().ToStdString();
		}

		if (!netlist.SaveToFile(m_currentFilePath.ToStdString())) {
			wxMessageBox("Failed to save", "Error", wxOK | wxICON_ERROR, this);
			return;
		}
	}
}

void cMain::OnOpen(wxCommandEvent& event) {
	wxString filename = wxFileSelector("Open File", wxEmptyString, wxEmptyString, "json", "JSON files (*.json)|*.json", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (!filename.empty()) {
		Netlist netlist;
		if (!netlist.LoadFromFile(filename.ToStdString())) {
			wxMessageBox("Failed to load circuit file", "Error", wxOK | wxICON_ERROR, this);
			return;
		}

		if (!NetlistUtils::ImportToCanvas(netlist, drawborad->m_components,
			drawborad->m_wires, drawborad->m_TotalComponentNum)) {
			wxMessageBox("Failed to import circuit", "Error", wxOK | wxICON_ERROR, this);
			return;
		}

		m_currentFilePath = filename;

		drawborad->Refresh();
	}
}

void cMain::OnClear(wxCommandEvent& event) {
	drawborad->m_components.clear();
	drawborad->m_wires.clear();
	drawborad->texts.clear();
	wxClientDC dc(drawborad);
	dc.Clear();
	drawborad->Refresh();
}

void cMain::OnZoomIn(wxCommandEvent&)
{
	for (const auto& comp : drawborad->m_components) {
		if (comp->GetSelected()) {
			if (comp->scale <= 1.5) {
				comp->scale = comp->scale + 0.1;
				comp->InitializePorts();
				comp->UpdateGeometry();
			}
			
		}
	}
	drawborad->Refresh();
	// 这里写放大逻辑
}

void cMain::OnZoomOut(wxCommandEvent&)
{
	for (const auto& comp : drawborad->m_components) {
		if (comp->GetSelected()) {
			if (comp->scale >=0.5) {
				comp->scale = comp->scale - 0.1;
				comp->InitializePorts();
				comp->UpdateGeometry();
			}
		}
	}
	drawborad->Refresh();
	// 这里写缩小逻辑
}

void cMain::OnDeleteSelected(wxCommandEvent& event) {
	drawborad->DeleteSelectedComponents();
}