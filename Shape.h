#pragma once
#include <wx/wx.h>
#include <wx/graphics.h>

enum class GateType {
    AND,
    OR,
    NOT,
    BUFFER,
    NAND,
    NOR,
    XOR,
    XNOR,
    ODD_PARITY,
    EVEN_PARITY,
    CONTROLLED_BUFFER,
    CONTROLLED_INVERTOR
};

enum class BaseType {
    TEXT_TOOL
};

class Shape
{
protected:
  
    wxPoint startPos;
    wxPoint endPos;
    int Thickness;
    wxColour Color;
    wxColour brushColor;
    wxString ShapeType;
public:
    Shape() : Thickness(2), Color(*wxBLACK) {}
    virtual ~Shape() {}

    wxPoint GetStart() const { return startPos; }
    wxPoint GetEnd() const { return endPos; }
    int GetThickness() const { return Thickness; }
    wxColour GetColor() const { return Color; }
    wxColour GetBrushColor() const { return brushColor; }
    wxString GetType() const { return ShapeType; }

    virtual void drawSelf(wxGraphicsContext* gc) const = 0;
};

// 新增 LogicGate 父类
class LogicGate : public Shape
{
protected:
    GateType type;
public:
    LogicGate(const wxPoint& pos, GateType t) : Shape(), type(t)
    {
        startPos = pos;
    }
    GateType GetGateType() const { return type; }
  

};

//新增文本框
class ComponentText : public Shape {
private:
    wxString text;              // 文本内容
    wxFont font;                // 字体
    bool isEditing;             // 是否处于编辑状态
    wxTextCtrl* textCtrl;       // 编辑用的文本控件
    wxWindow* parent;           // 父窗口（画布）

public:
    ComponentText(wxWindow* parentWnd, const wxPoint& pos);
    virtual ~ComponentText();

    void StartEditing(wxWindow* canvas);   // 开始编辑
    void FinishEditing();                  // 结束编辑
    void SetText(const wxString& newText); // 设置文字
    wxString GetText() const;              // 获取文字

    virtual void drawSelf(wxGraphicsContext* gc) const override;
};

// 具体门类
class ComponentAnd : public LogicGate
{
public:
    ComponentAnd(const wxPoint& pos) : LogicGate(pos, GateType::AND) { ShapeType = "And"; }
    virtual void drawSelf(wxGraphicsContext* gc) const override;
};

class ComponentNot : public LogicGate
{
public:
    ComponentNot(const wxPoint& pos) : LogicGate(pos, GateType::NOT) { ShapeType = "Not"; }
    virtual void drawSelf(wxGraphicsContext* gc) const override;
};

class ComponentOr : public LogicGate
{
public:
    ComponentOr(const wxPoint& pos) : LogicGate(pos, GateType::OR) { ShapeType = "Or"; }
    virtual void drawSelf(wxGraphicsContext* gc) const override;
};

class ComponentBuffer : public LogicGate
{
public:
    ComponentBuffer(const wxPoint& pos) : LogicGate(pos, GateType::BUFFER) { ShapeType = "Buffer"; }
    virtual void drawSelf(wxGraphicsContext* gc) const override;
};

class ComponentNAnd : public LogicGate
{
public:
    ComponentNAnd(const wxPoint& pos) : LogicGate(pos, GateType::NAND) { ShapeType = "NAnd"; }
    virtual void drawSelf(wxGraphicsContext* gc) const override;
};

class ComponentNOr : public LogicGate
{
public:
    ComponentNOr(const wxPoint& pos) : LogicGate(pos, GateType::NOR) { ShapeType = "Nor"; }
    virtual void drawSelf(wxGraphicsContext* gc) const override;
};

class ComponentXOr : public LogicGate
{
public:
    ComponentXOr(const wxPoint& pos) : LogicGate(pos, GateType::XOR) { ShapeType = "XOr"; }
    virtual void drawSelf(wxGraphicsContext* gc) const override;
};

class ComponentXNOr : public LogicGate
{
public:
    ComponentXNOr(const wxPoint& pos) : LogicGate(pos, GateType::XNOR) { ShapeType = "XNor"; }
    virtual void drawSelf(wxGraphicsContext* gc) const override;
};

class ComponentOddParity : public LogicGate
{
public:
    ComponentOddParity(const wxPoint& pos) : LogicGate(pos, GateType::ODD_PARITY) { ShapeType = "Odd Parity"; }
    virtual void drawSelf(wxGraphicsContext* gc) const override;
};

class ComponentEvenParity : public LogicGate
{
public:
    ComponentEvenParity(const wxPoint& pos) : LogicGate(pos, GateType::EVEN_PARITY) { ShapeType = "Even Parity"; }
    virtual void drawSelf(wxGraphicsContext* gc) const override;
};

class ComponentControlledBuffer : public LogicGate
{
public:
    ComponentControlledBuffer(const wxPoint& pos) : LogicGate(pos, GateType::CONTROLLED_BUFFER) { ShapeType = "Controlled Buffer"; }
    virtual void drawSelf(wxGraphicsContext* gc) const override;
};

class ComponentControlledInvertor : public LogicGate
{
public:
    ComponentControlledInvertor(const wxPoint& pos) : LogicGate(pos, GateType::CONTROLLED_INVERTOR) { ShapeType = "Controlled Invertor"; }
    virtual void drawSelf(wxGraphicsContext* gc) const override;
};

class Line : public Shape
{

public:
    Line(const wxPoint& start, const wxPoint& end) : Shape()
    {
        this->startPos = start; this->endPos = end;
    }
    virtual void drawSelf(wxGraphicsContext* gc) const override;
};
