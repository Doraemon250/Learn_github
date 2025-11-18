#include <wx/graphics.h>
#include "Component.h"

void Component::DrawPorts(wxDC& dc) const {
	dc.SetPen(*wxBLACK_PEN);
	dc.SetBrush(*wxGREEN_BRUSH);

	for (const auto& port : m_inputs) {
		wxPoint portPos = m_center + port.position;
		dc.DrawCircle(portPos, 3);
	}

	dc.SetBrush(*wxRED_BRUSH);
	for (const auto& port : m_outputs) {
		wxPoint portPos = m_center + port.position;
		dc.DrawCircle(portPos, 3);
	}
}

Port* Component::GetPortAt(wxPoint pt, bool& isInput) {
	for (auto& port : m_inputs) {
		wxPoint portPos = m_center + port.position;
		if (abs(pt.x - portPos.x) < 5 && abs(pt.y - portPos.y) < 5) {
			isInput = true;
			return &port;
		}
	}

	for (auto& port : m_outputs) {
		wxPoint portPos = m_center + port.position;
		if (abs(pt.x - portPos.x) < 5 && abs(pt.y - portPos.y) < 5) {
			isInput = false;
			return &port;
		}
	}

	return nullptr;
}

void ANDGate::InitializePorts() {
	m_inputs.clear();
	m_outputs.clear();
	m_inputs.push_back({ wxPoint(-40*scale, -10*scale), 0 });
	m_inputs.push_back({ wxPoint(-40*scale, 10*scale), 1 });
	m_outputs.push_back({ wxPoint(60*scale, 0), 0 });
}

void ANDGate::drawSelf(wxMemoryDC& memDC) {

	wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);

	gc->SetPen(wxPen(wxColour(0, 0, 0), 3));
	if (gc) {
		wxGraphicsPath path = gc->CreatePath();
		path.MoveToPoint(m_center.x + 20 * scale, m_center.y - 20 * scale);
		path.AddLineToPoint(m_center.x - 20 * scale, m_center.y - 20 * scale);
		path.AddLineToPoint(m_center.x - 20 * scale, m_center.y + 20 * scale);
		path.AddLineToPoint(m_center.x + 20 * scale, m_center.y + 20 * scale);
		path.AddArc(m_center.x + 20 * scale, m_center.y, 20 * scale, M_PI / 2, -M_PI / 2, false);

		path.MoveToPoint(m_center.x - 40 * scale, m_center.y + 10 * scale);
		path.AddLineToPoint(m_center.x - 20 * scale, m_center.y + 10 * scale);
		path.MoveToPoint(m_center.x - 40 * scale, m_center.y - 10 * scale);
		path.AddLineToPoint(m_center.x - 20 * scale, m_center.y - 10 * scale);
		path.MoveToPoint(m_center.x + 40 * scale, m_center.y);
		path.AddLineToPoint(m_center.x + 60 * scale, m_center.y);

		gc->StrokePath(path);
		if (m_isSelected)
		{
			gc->SetPen(wxPen(wxColour(128, 128, 128), 2));
			for (int j = 0; j < 4; j++) {
				gc->DrawEllipse(m_BoundaryPoints[j].x - 5, m_BoundaryPoints[j].y - 5, 8, 8);
			}
		}
	}
	delete gc;
	DrawPorts(memDC);

}

bool ANDGate::Isinside(const wxPoint& point) const
{
	int left = m_BoundaryPoints[0].x;
	int right = m_BoundaryPoints[2].x;
	int top = m_BoundaryPoints[0].y;
	int bottom = m_BoundaryPoints[1].y;
	return (point.x >= left && point.x <= right &&
		point.y >= top && point.y <= bottom);
}


void ANDGate::UpdateGeometry() {
	m_BoundaryPoints[0] = wxPoint(m_center.x - 40 * scale, m_center.y - 20 * scale);//×óÉÏ
	m_BoundaryPoints[1] = wxPoint(m_center.x - 40 * scale, m_center.y + 20 * scale);//×óÏÂ
	m_BoundaryPoints[2] = wxPoint(m_center.x + 60 * scale, m_center.y - 20 * scale);//ÓÒÉÏ
	m_BoundaryPoints[3] = wxPoint(m_center.x + 60 * scale, m_center.y + 20 * scale);//ÓÒÏÂ
}




void ORGate::drawSelf(wxMemoryDC& memDC) {
	wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
	gc->SetPen(wxPen(wxColour(0, 0, 0), 3));
	if (gc) {
		wxGraphicsPath path = gc->CreatePath();
		path.MoveToPoint(m_center.x, m_center.y - 20 * scale);
		path.AddLineToPoint(m_center.x - 20 * scale, m_center.y - 20 * scale);
		path.AddQuadCurveToPoint(m_center.x, m_center.y, m_center.x - 20 * scale, m_center.y + 20 * scale);
		path.AddLineToPoint(m_center.x - 10 * scale, m_center.y + 20 * scale);
		path.AddQuadCurveToPoint(m_center.x + 20 * scale, m_center.y + 20 * scale, m_center.x + 40 * scale, m_center.y);
		path.AddQuadCurveToPoint(m_center.x + 20 * scale, m_center.y - 20 * scale, m_center.x, m_center.y - 20 * scale);

		path.MoveToPoint(m_center.x - 40 * scale, m_center.y + 10 * scale);
		path.AddLineToPoint(m_center.x - 13 * scale, m_center.y + 10 * scale);
		path.MoveToPoint(m_center.x - 40 * scale, m_center.y - 10 * scale);
		path.AddLineToPoint(m_center.x - 13 * scale, m_center.y - 10 * scale);
		path.MoveToPoint(m_center.x + 40 * scale, m_center.y);
		path.AddLineToPoint(m_center.x + 60 * scale, m_center.y);

		gc->StrokePath(path);
	}
	delete gc;
}