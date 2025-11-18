
#include <wx/wx.h>
#include <wx/graphics.h>

enum ToolType {
	TOOL_WIRE,
	TOOL_TEXT,
	TOOL_IDEL,
};

enum ComponentType {
	ANDGATE,
	ORGATE,
	NOTGATE,
	NONE_TYPE
};

struct Port {
	wxPoint position;
	int index;
};

class Wire {
public:
	wxPoint start;
	wxPoint end;
	int startComponentId;
	int endComponentId;
	int startPortIndex;
	int endPortIndex;
	bool isActive;

	Wire() : startComponentId(-1), endComponentId(-1),
		startPortIndex(-1), endPortIndex(-1), isActive(false) {
	}

	void drawSelf(wxMemoryDC& memDC) const {
		wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
		gc->SetPen(wxPen(isActive ? wxColour(255, 0, 0) : wxColour(0, 128, 0), 2));
		gc->StrokeLine(start.x, start.y, end.x, end.y);
	}
};

class Component {
public:
	int m_id;
	ComponentType m_type;
	double scale;
	bool scaling = false;
	wxPoint m_BoundaryPoints[4];
	std::vector<Port> m_inputs;
	std::vector<Port> m_outputs;


	Component(wxPoint center, ComponentType type, int id) {
		this->scale = 1.0;
		this->m_type = type;
		this->m_center = center;
		this->m_isSelected = false;
		this->m_id = id;
	}
	wxPoint GetCenter() { return m_center; }
	void SetCenter(wxPoint center) { m_center = center; UpdateGeometry(); }
	void SetSelected(bool selected) { m_isSelected = selected; }
	bool GetSelected() { return m_isSelected; }
	virtual void UpdateGeometry() = 0;
	virtual void drawSelf(wxMemoryDC& memDC) = 0;
	virtual bool Isinside(const wxPoint& point) const = 0;

	virtual void InitializePorts() = 0;
	void DrawPorts(wxDC& dc) const;
	Port* GetPortAt(wxPoint pt, bool& isInput);
protected:
	bool m_isSelected;
	wxPoint m_center;
private:

};

class Gate :public Component {
public:
	wxPoint pout;
	Gate(wxPoint center, ComponentType type, int id) :Component(center, type, id) {};
	virtual ~Gate() {}
};

class ANDGate :public Gate {
public:

	ANDGate(wxPoint center, ComponentType type, int id) :Gate(center, type, id) {
		this->scale = 1.0;
		UpdateGeometry();
		InitializePorts();
	}
	virtual void drawSelf(wxMemoryDC& memDC)   override;
	virtual bool Isinside(const wxPoint& point) const override;
	virtual void InitializePorts() override;
	void UpdateGeometry()  override;
};

class ORGate :public Gate {
public:
	ORGate(wxPoint center, ComponentType type, int id) :Gate(center, type, id) {
		scale = 1.0;
	}
	virtual void drawSelf(wxMemoryDC& memDC)   override;
};

