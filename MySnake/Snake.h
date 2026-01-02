#pragma once

#include "common.h"
#include "Map.h"

#include <Time.h>
#include <list>

#define SNAKE_WIDTH TILE_SIZE //蛇节点的宽px---图片大小是20px*20px

//方向枚举体
enum Dir {
	DIR_UP = 1,
	DIR_RIGHT = 2,
	DIR_DOWN = 3,
	DIR_LEFT = 4
};

//蛇的节点类
class Segment
{
public:
	int x, y;//节点在地图中的坐标--以格子为单位
	Dir dir;
	Segment() :x(-1), y(-1) {}
	Segment(int newx, int newy, Dir newdir) :x(newx), y(newy), dir(newdir) {};
	bool operator ==(Segment &other)//重载==符号
	{
		return (x == other.x && y == other.y);
	}
};


class Snake
{
public:
	const int MinSpeed = 1;			//蛇的最小速度
	const int MaxSpeed = 25;		//蛇的最大速度
	const int OrgSpeed = 15;		//蛇的原始速度
private:
	int m_len;						//蛇的长度
	int m_speed;					//蛇的速度
	long t;                         //蛇的时间
	int m_step = 0;                 //蛇的步数
	Dir m_direction;				//蛇的方向
	std::list<Segment> m_snakelist;	//蛇的链表
	Segment m_tail;					//蛇移动过后的尾部节点，主要用于吃食物

	//蛇身体节点的图片
	 IMAGE headDown, headUp, headRight, headLeft;
	 IMAGE tailDown, tailUp, tailRight, tailLeft;
	 IMAGE bodyVerticalImg, bodyHorizontalImg;
	 IMAGE turnUpRightImg, turnRightDownImg, turnDownLeftImg, turnLeftUpImg;

public:
	Snake();

	void loadImages();		//加载蛇的图片

	int getLen();					//获取长度
	int getSpeed();					//获取速度
	Dir getDirection();				//获取方向
	std::list<Segment> GetSnakeAllNode();//获取整个蛇的位置v
	IMAGE& getSegmentImage(const Segment& curr, const Segment& pre);//获取蛇身体的图片

	bool setSpeed(int speed);		//设置速度，设置成功返回true

	void Move();					//移动一节
	void EatFood();					//吃食物
	void ChangeDir(Dir dir);		//改变方向
	void Dead();					//死亡

	bool ColideWall(Map& map);	//判断是否碰墙
	bool ColideSnake();											//碰撞到了自身
	bool ColideFood(int foodx,int foody);						//碰到了食物

	void DrawSnake(Map& map);											//绘制蛇
	~Snake();
};

