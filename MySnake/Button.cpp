#include "Button.h"
using namespace std;


void Button::drawButton(COLORREF fillColor, int textHeight, bool isPressed)
{
    // 备份
    int oldfillcolor = getfillcolor();
    int oldbkcolor = getbkcolor();

    // 绘制圆角矩形
    setfillcolor(fillColor);
    fillroundrect(dimensions.left, dimensions.top, dimensions.right, dimensions.bottom, dimensions.radius, dimensions.radius);

    // 打印文本内容
    settextcolor(color2);
    setbkmode(TRANSPARENT);
    settextstyle(textHeight, 0, L"微软雅黑");
    drawtext(text.c_str(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // 恢复
    setfillcolor(oldfillcolor);
    setbkcolor(oldbkcolor);
}

void Button::eventConnect(CallBack newEvent)
{
    event = newEvent;
}

bool Button::handleEvent(ExMessage& msg)
{
    switch (msg.message) {
    case WM_MOUSEMOVE:
        if (msg.x > dimensions.left && msg.x < dimensions.right && msg.y > dimensions.top && msg.y < dimensions.bottom)
            state = BUTTON_ONTOP;
        else {
            state = (state == BUTTON_ONTOP) ? BUTTON_OFFTOP : BUTTON_OFFCLICK;
        }
        break;
    case WM_LBUTTONDOWN:
        if (state == BUTTON_ONTOP)
            state = BUTTON_ONCLICK;
        else
            state = BUTTON_OFFCLICK;
        break;
    default:
        break;
    }

    switch (state)
    {
    case Button::BUTTON_ONCLICK:
        drawButton(color1-0x110010, nwidth*2/3, true);
        
        if (event != nullptr)
        {
            event(); // 执行绑定的回调函数
        }
        state = BUTTON_USEDONTOP;
        return true;
        break;
    case Button::BUTTON_OFFCLICK:
        break;
    case Button::BUTTON_ONTOP:
        drawButton(color1 - 0x111111, nwidth);
        break;
    case Button::BUTTON_OFFTOP:
        drawButton(color1, nwidth);
        state = BUTTON_OFFCLICK;
        break;
    case Button::BUTTON_USEDONTOP:
        state = BUTTON_ONTOP;
        break;
    default:
        break;
    }
    return false;
}

Button::Button() : state(BUTTON_OFFCLICK), event(nullptr) {
    // 初始化成员变量
    dimensions = { 0, 0, 0, 0, 0 };
    nwidth = 0;
    color1 = 0;
    color2 = 0;
    rect = { 0, 0, 0, 0 };
}

Button::Button(int x1, int y1, int x2, int y2, int r, const TCHAR* s, COLORREF c, COLORREF c1)
    : state(BUTTON_OFFCLICK), dimensions({ x1, y1, x2, y2, r }), text(s), color1(c), color2(c1), event(nullptr) {
    rect = { x1, y1, x2, y2 };
    nwidth = y2 - y1;

    settextstyle(nwidth, 0, _T("微软雅黑"));
    if (textwidth(text.c_str()) > (dimensions.right - dimensions.left)) {
        nwidth = (dimensions.right - dimensions.left) * nwidth / textwidth(text.c_str());
    }

    drawButton(color1, nwidth);
}