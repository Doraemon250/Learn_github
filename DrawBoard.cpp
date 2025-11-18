#include "DrawBoard.h"
#include "TextInputDialog.h"
#include "text.h"

bool TextDropTarget::OnDropText(wxCoord x, wxCoord y, const wxString& text)
{
    if (text == "ÓëÃÅ")
    {
        m_canvas->AddGate(x, y, ANDGATE, "ÓëÃÅ");
        return true;
    }
    return false;
}

void DrawBoard::AddGate(int x, int y, ComponentType type, const wxString& name)
{
    m_mousePos.x = x;
    m_mousePos.y = y;
    Gate* gate = new ANDGate(m_mousePos, ANDGATE, m_TotalComponentNum++);
    AddComponent(gate);
    Refresh();
}

DrawBoard::DrawBoard(wxPanel* parent) : wxPanel(parent), m_TotalComponentNum(0), m_dragging(false) {

    SetBackgroundStyle(wxBG_STYLE_PAINT);
    Bind(wxEVT_PAINT, &DrawBoard::OnPaint, this);
    Bind(wxEVT_MOTION, &DrawBoard::OnButtonMove, this);
    Bind(wxEVT_LEFT_DOWN, &DrawBoard::OnLeftDown, this);
    Bind(wxEVT_LEFT_UP, &DrawBoard::OnLeftUp, this);

    st1 = new wxStaticText(this, -1, wxT(""), wxPoint(10, 10));
    st2 = new wxStaticText(this, -1, wxT(""), wxPoint(10, 30));

    SetIdleMode();
    SetDropTarget(new TextDropTarget(this));

}

DrawBoard::~DrawBoard() {
    for (Component* component : m_components) {
        delete component;
    }

}

void DrawBoard::OnPaint(wxPaintEvent& event) {
    wxAutoBufferedPaintDC dc(this);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();

    wxBitmap bufferBitmap(GetClientSize());
    wxMemoryDC memDC;
    memDC.SelectObject(bufferBitmap);
    memDC.SetBackground(*wxWHITE_BRUSH);
    memDC.Clear();


    wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
    if (gc) {
        drawGrid(gc);

        for (const auto& component : m_components) {
            component->drawSelf(memDC);
        }
        for (const auto& wire : m_wires) {
            wire.drawSelf(memDC);
        }
        for (const std::unique_ptr<Text>& text : texts) {
            text->drawText(gc);
        }
        gc->SetBrush(wxBrush(wxColour(0, 0, 0)));
        gc->SetPen(wxPen(wxColour(0, 0, 0), 1));
        gc->StrokeLine(m_mousePos.x, 0, m_mousePos.x, GetSize().y);
        gc->StrokeLine(0, m_mousePos.y, GetSize().x, m_mousePos.y);

        if (m_isDrawingLine) {
            gc->SetPen(wxPen(wxColour(255, 0, 0), 2, wxPENSTYLE_DOT));
            gc->StrokeLine(m_wireStartPos.x, m_wireStartPos.y, m_mousePos.x, m_mousePos.y);
        }

        delete gc;
    }

    dc.Blit(0, 0, GetClientSize().GetWidth(), GetClientSize().GetHeight(), &memDC, 0, 0);

}


void DrawBoard::drawGrid(wxGraphicsContext* gc) {

    wxSize size = GetSize();
    int gridSize = 20;

    gc->SetPen(wxPen(wxColour(180, 180, 180), 1, wxPENSTYLE_DOT_DASH));
    for (int x = 0; x < size.x; x += gridSize) {
        gc->StrokeLine(x, 0, x, size.y);
    }

    for (int y = 0; y < size.y; y += gridSize) {
        gc->StrokeLine(0, y, size.x, y);
    }
}

void DrawBoard::OnButtonMove(wxMouseEvent& event) {
    wxPoint prev = m_mousePos;
    m_mousePos = event.GetPosition();
    st1->SetLabel(wxString::Format(wxT("x: %d"), event.GetX()));
    st2->SetLabel(wxString::Format(wxT("y: %d"), event.GetY()));

    if (m_dragging && m_selectedElement && m_currentTool == TOOL_IDEL) {
        wxPoint offset(m_mousePos.x - prev.x, m_mousePos.y - prev.y);
        wxPoint center = m_selectedElement->GetCenter();
        m_selectedElement->SetCenter(center + offset);

        int compId = m_selectedElement->m_id;
        for (auto& wire : m_wires) {
            if (wire.startComponentId == compId) {
                for (const auto& comp : m_components) {
                    if (comp->m_id == compId) {
                        wire.start = comp->GetCenter() + comp->m_outputs[wire.startPortIndex].position;
                        break;
                    }
                }
            }
            if (wire.endComponentId == compId) {
                for (const auto& comp : m_components) {
                    if (comp->m_id == compId) {
                        wire.end = comp->GetCenter() + comp->m_inputs[wire.endPortIndex].position;
                        break;
                    }
                }
            }
        }
    }
    Refresh();
}

void DrawBoard::OnLeftDown(wxMouseEvent& event) {

    m_mousePos = event.GetPosition();
    switch (m_currentTool) {
    case TOOL_TEXT: {
        m_wireStartPos = event.GetPosition();
        TextInputDialog dialog(this, wxID_ANY, "Ìí¼Ó±êÇ©");

        if (dialog.ShowModal() == wxID_OK) {
            wxString userInput = dialog.GetText();
            double angleInput = dialog.GetAngle();

            std::unique_ptr<Text> textPtr = std::make_unique<Text>(m_wireStartPos, userInput);
            textPtr->setAngle(angleInput);
            texts.push_back(move(textPtr));
        }
        break;
    }
    case TOOL_WIRE: {
        if (!m_isDrawingLine) {
            for (const auto& comp : m_components) {
                bool isInput;
                Port* port = comp->GetPortAt(m_mousePos, isInput);
                if (port) {
                    if (!isInput) {
                        m_isDrawingLine = true;
                        m_wireStartPos = comp->GetCenter() + port->position;
                        m_wireStartComponentId = comp->m_id;
                        m_wireStartPortIndex = port->index;
                        m_wireStartIsOutput = true;

                        //if (m_parentFrame) {
                        //    m_parentFrame->SetStatusText("Wiring started");
                        //}
                        break;
                    }
                    //else if (m_parentFrame) {
                    //    m_parentFrame->SetStatusText("Must start from output port");
                    //}
                }
            }
        }
        break;
    }
    case TOOL_IDEL: {
        wxPoint curpos = event.GetPosition();
        m_selectedElement = nullptr;
        for (auto& element : m_components) {
            element->SetSelected(false);
        }
        for (auto& element : m_components) {
            if (element->Isinside(curpos))
            {
                element->SetSelected(true);
                m_selectedElement = element;
                m_dragging = true;
                break;
            }
        }
        break;
    }
    }
    Refresh();

}

void DrawBoard::AddComponent(Component* component) {
    m_components.push_back(component);
    Refresh();
}

void DrawBoard::DeleteSelectedComponents() {
    if (!m_selectedElement)
        return;

    auto it = std::find(m_components.begin(), m_components.end(), m_selectedElement);
    if (it != m_components.end())
    {
        delete* it;
        m_components.erase(it);
    }
    m_selectedElement = nullptr;
    Refresh();
}

void DrawBoard::SetIdleMode() {
    m_currentTool = TOOL_IDEL;
    m_isDrawingLine = false;
}

void DrawBoard::OnLeftUp(wxMouseEvent& event) {
    m_mousePos = event.GetPosition();

    if (m_dragging) {
        //ReleaseMouse();
        m_dragging = false;
        //MarkDirty();
    }

    if (m_isDrawingLine && m_currentTool == TOOL_WIRE) {
        bool wireCreated = false;
        for (const auto& comp : m_components) {
            bool isInput;
            Port* port = comp->GetPortAt(m_mousePos, isInput);
            if (port && isInput && comp->m_id != m_wireStartComponentId) {
                Wire wire;
                wire.start = m_wireStartPos;
                wire.end = comp->GetCenter() + port->position;
                wire.startComponentId = m_wireStartComponentId;
                wire.endComponentId = comp->m_id;
                wire.startPortIndex = m_wireStartPortIndex;
                wire.endPortIndex = port->index;
                m_wires.push_back(wire);
                wireCreated = true;

                //MarkDirty();

                //if (m_parentFrame) {
                //    m_parentFrame->SetStatusText("Wire connected");
                //}
                break;
            }
        }

        //if (!wireCreated && m_parentFrame) {
          //  m_parentFrame->SetStatusText("Wire cancelled");
        //}

        m_isDrawingLine = false;
        m_wireStartComponentId = -1;
        //m_bufferValid = false;
        Refresh();
    }
}