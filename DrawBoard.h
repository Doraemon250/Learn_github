#pragma once
#include <wx/wx.h>
#include <wx/dcgraph.h>
#include <wx/dcbuffer.h>
#include <wx/textctrl.h>
#include <wx/gdicmn.h>
#include <wx/dnd.h>
#include "text.h"
#include "Component.h"

class DrawBoard : public wxPanel
{
public:
	int m_TotalComponentNum;
	ToolType m_currentTool;
	wxPoint m_mousePos;
	wxPoint m_wireStartPos;
	//wxPoint currentEnd;
	wxStaticText* st1;
	wxStaticText* st2;
	wxPen Pen;

	std::vector<std::unique_ptr<Text>> texts;

	std::vector<Component*> m_components;        // 元件列表
	std::vector<Wire> m_wires;

	//bool AtStart;

	int m_wireStartComponentId;
	int m_wireStartPortIndex;
	bool m_wireStartIsOutput;

	void OnPaint(wxPaintEvent& event);
	void OnButtonMove(wxMouseEvent& event);
	void OnLeftDown(wxMouseEvent& event);
	void OnLeftUp(wxMouseEvent& event);
	void drawGrid(wxGraphicsContext* gc);
	void AddGate(int x, int y, ComponentType type, const wxString& name);

	void SetIdleMode();
	void DeleteSelectedComponents();
	void AddComponent(Component* component);

	bool GetisDrawingLine() { return m_isDrawingLine; }
	void SetisDrawingLine(bool DrawingLine) { m_isDrawingLine = DrawingLine; }
	void SetCurrentTool(ToolType tool) { m_currentTool = tool; }

public:
	DrawBoard(wxPanel* parent);
	~DrawBoard();
private:
	bool m_isDrawingLine;
	//bool m_isDrawingText;	
	Component* m_selectedElement; // 当前选中
	bool m_dragging = false;

};

class TextDropTarget : public wxTextDropTarget
{
public:
	TextDropTarget(DrawBoard* canvas) : m_canvas(canvas) {}
	virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& text) override;
private:
	DrawBoard* m_canvas;
};