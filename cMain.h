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



	void OnSave(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnClear(wxCommandEvent& event);

	void OnEnableDrawAndGate(wxCommandEvent& evt);
	void OnEnableDrawOrGate(wxCommandEvent& evt);
	void OnDrawText(wxCommandEvent& event);
	void OnDrawIdle(wxCommandEvent& event);
	void OnDrawLine(wxCommandEvent& event);
private:
	void OnZoomIn(wxCommandEvent& evt);
	void OnZoomOut(wxCommandEvent& evt);
	void OnDeleteSelected(wxCommandEvent& event);

	//wxDECLARE_EVENT_TABLE();
};

