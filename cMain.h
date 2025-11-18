#pragma once
#include <wx/wx.h>
#include <fstream>
#include <iomanip>
#include "DrawBoard.h"
#include "TreePanel.h"

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();

public:
	wxString m_currentFilePath;
	DrawBoard* drawborad;

	wxToolBar* right_toolBar;

	void OnSave(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnClear(wxCommandEvent& event);

	void OnEnableDrawAndGate(wxCommandEvent& evt);
	void OnEnableDrawOrGate(wxCommandEvent& evt);
	void OnDrawText(wxCommandEvent& event);
	void OnDrawIdle(wxCommandEvent& event);
	void OnDrawLine(wxCommandEvent& event);
	void ResetToolIcons();  // 新增：重置工具按钮为默认图标
private:
	void OnZoomIn(wxCommandEvent& evt);
	void OnZoomOut(wxCommandEvent& evt);
	void OnDeleteSelected(wxCommandEvent& event);

	//wxDECLARE_EVENT_TABLE();
};



