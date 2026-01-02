#pragma once
#include "common.h"

#define TILE_SIZE 15	//每个草方块的大小20px*20px---和蛇节点的大小相呼应--要改两个都得改动

#define GLASS_COLOR1  RGB(170, 215, 81)  // 默认草地颜色1
#define GLASS_COLOR2  RGB(162, 209, 73)  // 默认草地颜色2

class Map
{
private:
	int m_x, m_y;		//地图左上角坐标
	int m_width;		//地图宽	单位是方块；
	int m_height;		//地图高	单位是方块；
	COLORREF m_color1;	//草的颜色1
	COLORREF m_color2;	//草的颜色2
public:
	Map(int x=0, int y=0, int w = 480/ TILE_SIZE, int h = 480/ TILE_SIZE, COLORREF color1 = GLASS_COLOR1, COLORREF color2 = GLASS_COLOR2);
	int getWidth();
	int getHeight();
	void drawALLMap();			//画出整个地图
	void drawBlock(int x,int y);//画单个方块--画蛇的时候用到
	~Map();

	friend class Snake;
	friend class Food;
};


