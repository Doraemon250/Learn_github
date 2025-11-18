#include "text.h"

Text::Text(wxPoint origin, wxString content) {
    this->origin = origin;
    this->content = content;
    colour = wxColour(0, 100, 100);
    this->font.SetPointSize(12);
    this->font.SetFamily(wxFONTFAMILY_DEFAULT);
    this->font.SetStyle(wxFONTSTYLE_NORMAL);
    this->font.SetWeight(wxFONTWEIGHT_BOLD);
    this->font.SetUnderlined(false);
    this->font.SetFaceName(wxT("Arial"));
    this->font.SetEncoding(wxFONTENCODING_DEFAULT);
}

Text::Text(wxPoint origin, wxString content, const wxColour& colour, const wxFont font) {
    this->origin = origin;
    this->content = content;
    this->colour = colour;
    this->font = font;
}


void Text::drawText(wxGraphicsContext* gc) {
    //默认顺时针旋转

    //wxFont font(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    gc->SetFont(font, colour);

    // 计算旋转后的文本w位置
    double textX = cos(wxDegToRad(angle)) * origin.x + sin(wxDegToRad(angle)) * origin.y;
    double textY = -sin(wxDegToRad(angle)) * origin.x + cos(wxDegToRad(angle)) * origin.y;

    gc->PushState();  // 保存当前状态

    // 旋转坐标系
    gc->Rotate(wxDegToRad(angle));

    //获取文本的宽度和高度
    double textWidth, textHeight, descent, externalLeading;
    gc->GetTextExtent(content, &textWidth, &textHeight, &descent, &externalLeading);

    //width = -cos(wxDegToRad(angle)) * textWidth - sin(wxDegToRad(angle)) * textHeight;
    //height = sin(wxDegToRad(angle)) * textWidth - cos(wxDegToRad(angle)) * textHeight;

    // 绘制垂直文本
    gc->DrawText(content, textX, textY);
    gc->PopState();  // 恢复到之前的状态
}

void Text::drawText(wxGraphicsContext* gc, wxPoint position) {
    gc->SetFont(font, colour);
    double textX = cos(wxDegToRad(angle)) * (origin.x + position.x) + sin(wxDegToRad(angle)) * (origin.y + position.y);
    double textY = -sin(wxDegToRad(angle)) * (origin.x + position.x) + cos(wxDegToRad(angle)) * (origin.y + position.y);
    gc->PushState();
    gc->Rotate(wxDegToRad(angle));
    double textWidth, textHeight, descent, externalLeading;
    gc->GetTextExtent(content, &textWidth, &textHeight, &descent, &externalLeading);
    gc->DrawText(content, textX, textY);
    gc->PopState();
}
