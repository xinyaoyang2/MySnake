#pragma once
#include "common.h"
#include "Snake.h"
#include "Map.h"

class Food
{
private:
	IMAGE foodImg;	//食物的png图片
	int x, y;		//食物的坐标
	bool m_state;	//食物的状态：false--没有了；true--还存在
public:
	Food();
	void loadImg();				//加载图片
	bool getState();			//获取食物当前状态
	int getx();
	int gety();

	void setState(bool new_state);	//设置食物状态
	void Generate(Map& map,std::list<Segment> seglist);//生成一个食物
	void drawFood(Map& map);
	~Food();
	
};
