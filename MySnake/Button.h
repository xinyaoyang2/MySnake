#pragma once
#include "common.h"
#include <string>
//回调函数
typedef void(*CallBack)();

//基于easyx的按钮控件
class Button
{
private:
	enum ButtonState {
		BUTTON_ONCLICK,		//按键被点击
		BUTTON_OFFCLICK,	//按键未被点击（曾经不在）
		BUTTON_ONTOP,		//鼠标在按键上
		BUTTON_OFFTOP,		//鼠标不在按键上(曾今在过)
		BUTTON_USEDONTOP	//鼠标上次在按键上
	}state;//按钮状态
	struct {
		int left;			//按键的左边框
		int top;			//按键的上边框
		int right;			//按键的右边框
		int bottom;			//按键的下边框
		int radius;			//按键的圆角半径
	}dimensions;			//按键尺寸

	int nwidth;				//字符高度
	RECT rect;				//矩形
	std::basic_string<TCHAR> text;//文本内容
	COLORREF color1;		//按钮颜色
	COLORREF color2;		//文本颜色
	
	CallBack event;			//按键按下后触发的事件，右外部给出
	
	//按键绘制
	void drawButton(COLORREF fillColor, int textHeight, bool isPressed=false);

public:
	Button();//默认构造函数--用于批量创建
	Button(int x1, int x2, int y1, int y2, int r, const TCHAR* s, COLORREF c, COLORREF c1);
	void eventConnect(CallBack newEvent);//事件绑定
	bool handleEvent(ExMessage& msg);//按键是否触发---放进消息循环中
	~Button() {};
};