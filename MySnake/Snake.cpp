#include "Snake.h"



Snake::Snake()
{
	Segment pos0(10, 11, DIR_RIGHT);
	Segment pos1(9, 11, DIR_RIGHT);
	Segment pos2(8, 11, DIR_RIGHT);

	this->m_snakelist.push_back(pos0);
	this->m_snakelist.push_back(pos1);
	this->m_snakelist.push_back(pos2);

	this->m_tail = { 7,11,Dir::DIR_RIGHT };

	this->m_direction = Dir::DIR_RIGHT;
	this->m_len = 3;
	this->m_speed = this->OrgSpeed;
	loadImages();
}

void Snake::loadImages()
{
	loadimage(&headDown, L"PNG", _T("head_down"), SNAKE_WIDTH, SNAKE_WIDTH, 1);
	loadimage(&headUp, L"PNG", _T("head_up"), SNAKE_WIDTH, SNAKE_WIDTH, 1);
	loadimage(&headRight, L"PNG", _T("head_right"), SNAKE_WIDTH, SNAKE_WIDTH, 1);
	loadimage(&headLeft, L"PNG", _T("head_left"), SNAKE_WIDTH, SNAKE_WIDTH,1);
	loadimage(&tailUp, L"PNG", _T("tail_up"), SNAKE_WIDTH, SNAKE_WIDTH, 1);
	loadimage(&tailDown, L"PNG", _T("tail_down"), SNAKE_WIDTH, SNAKE_WIDTH, 1);
	loadimage(&tailRight, L"PNG", _T("tail_right"), SNAKE_WIDTH, SNAKE_WIDTH, 1);
	loadimage(&tailLeft, L"PNG", _T("tail_left"), SNAKE_WIDTH, SNAKE_WIDTH, 1);

	loadimage(&bodyVerticalImg, L"PNG", _T("body_vertical"), SNAKE_WIDTH, SNAKE_WIDTH, 1);
	loadimage(&bodyHorizontalImg, L"PNG", _T("body_horizontal"), SNAKE_WIDTH, SNAKE_WIDTH, 1);
	loadimage(&turnUpRightImg, L"PNG", _T("body_topright"), SNAKE_WIDTH, SNAKE_WIDTH, 1);
	loadimage(&turnRightDownImg, L"PNG", _T("body_bottomright"), SNAKE_WIDTH, SNAKE_WIDTH, 1);
	loadimage(&turnDownLeftImg, L"PNG", _T("body_bottomleft"), SNAKE_WIDTH, SNAKE_WIDTH, 1);
	loadimage(&turnLeftUpImg, L"PNG", _T("body_topleft"), SNAKE_WIDTH, SNAKE_WIDTH, 1);
}

int Snake::getLen()
{
	return this->m_len;
}

int Snake::getSpeed()
{
	return this->m_speed;
}

Dir Snake::getDirection()
{
	return this->m_direction;
}

std::list<Segment> Snake::GetSnakeAllNode()
{
	return this->m_snakelist;
}

IMAGE& Snake::getSegmentImage(const Segment& curr, const Segment& pre)
{
	// 判断当前部分与下一个部分的方向
	if (curr.dir == DIR_UP && pre.dir == DIR_RIGHT) return turnRightDownImg;
	if (curr.dir == DIR_RIGHT && pre.dir == DIR_UP) return turnLeftUpImg;

	if (curr.dir == DIR_UP && pre.dir == DIR_LEFT) return turnDownLeftImg;
	if (curr.dir == DIR_LEFT && pre.dir == DIR_UP) return turnUpRightImg;

	if (curr.dir == DIR_DOWN && pre.dir == DIR_LEFT) return turnLeftUpImg;
	if (curr.dir == DIR_LEFT && pre.dir == DIR_DOWN) return turnRightDownImg;

	if (curr.dir == DIR_DOWN && pre.dir == DIR_RIGHT) return turnUpRightImg;
	if (curr.dir == DIR_RIGHT && pre.dir == DIR_DOWN) return turnDownLeftImg;

	// 如果没有拐弯，则判断是水平还是垂直
	if (curr.dir == DIR_UP || curr.dir == DIR_DOWN) return bodyVerticalImg;
	if (curr.dir == DIR_LEFT || curr.dir == DIR_RIGHT) return bodyHorizontalImg;

	// 默认返回水平身体图片
	return bodyHorizontalImg;
}

bool Snake::setSpeed(int speed)
{
	if (speed > this->MaxSpeed)
	{
		if (this->m_speed != this->MaxSpeed)
		{
			this->m_speed = this->MaxSpeed;
			return true;
		}
		return false;
	}
	else if (speed < this->MinSpeed)
	{
		if (this->m_speed != this->MinSpeed)
		{
			this->m_speed = this->MinSpeed;
			return true;
		}
		return false;
	}
	else
	{
		this->m_speed = speed;
		return true;
	}
}

void Snake::Move()
{
	// 记录尾部位置
	m_tail = m_snakelist.back();

	// 移除最后一个节点
	m_snakelist.pop_back();

	// 获取并修改头部位置
	Segment head = m_snakelist.front();
	head.dir = m_direction;

	switch (m_direction) {
	case Dir::DIR_UP:
		head.y -= 1;
		break;
	case Dir::DIR_RIGHT:
		head.x += 1;
		break;
	case Dir::DIR_DOWN:
		head.y += 1;
		break;
	case Dir::DIR_LEFT:
		head.x -= 1;
		break;
	}

	// 将新头部位置插入到前面
	m_snakelist.push_front(head);
}

void Snake::EatFood()
{
	this->m_snakelist.push_back(m_tail);
	m_len++;
}

void Snake::ChangeDir(Dir dir)
{
	switch (dir)
	{
	case Dir::DIR_UP:
	case Dir::DIR_DOWN:
		if (m_direction != Dir::DIR_UP && m_direction != Dir::DIR_DOWN)
		{
			m_direction = dir;
		}
		break;
	case Dir::DIR_RIGHT:
	case Dir::DIR_LEFT:
		if (m_direction != Dir::DIR_RIGHT && m_direction != Dir::DIR_LEFT)
		{
			m_direction = dir;
		}
		break;
	}
}

void Snake::Dead()
{
	;
}

bool Snake::ColideWall(Map& map)
{
	Segment& head = m_snakelist.front();
	return (head.x < 0 || head.x >= map.m_width || head.y < 0 || head.y >= map.m_height);
}

bool Snake::ColideSnake()
{
	if (m_len <= 3) return false;
	std::list<Segment>::iterator it = this->m_snakelist.begin();

	Segment pos = *it;
	Segment next_pos;
	it++;

	while (it != this->m_snakelist.end())
	{
		next_pos = *it;

		if (pos == next_pos)
		{
			return true;
		}

		it++;
	}
	return false;
}

bool Snake::ColideFood(int foodx, int foody)
{
	Segment head = this->m_snakelist.front();
	if (head.x == foodx && head.y == foody)//蛇头如果撞到传入食物坐标
	{
		return true;
	}
	return false;
}

void Snake::DrawSnake(Map& map)
{
	if (m_snakelist.empty()) return;

	// 绘制蛇头
	const Segment& head = m_snakelist.front();

	//贴图之前先清除之前的图形
	map.drawBlock(head.x, head.y);

	IMAGE head_img;
	switch (head.dir) {
	case DIR_UP: head_img = headUp; break;
	case DIR_RIGHT: head_img = headRight; break;
	case DIR_DOWN: head_img = headDown; break;
	case DIR_LEFT: head_img = headLeft; break;
	}
	drawAlpha(&head_img, map.m_x+head.x*SNAKE_WIDTH, map.m_y+head.y* SNAKE_WIDTH);


	// 绘制蛇身体
	std::list<Segment>::iterator pre = m_snakelist.begin();
	std::list<Segment>::iterator it = std::next(pre);
	for (int i = 1; (i < m_snakelist.size() - 1) && it != m_snakelist.end(); i++, ++it) {
		IMAGE& img = getSegmentImage(*it,*pre);
		//贴图之前先清除之前的图形
		map.drawBlock(it->x, it->y);
		drawAlpha(&img, map.m_x+(it->x)* SNAKE_WIDTH, map.m_y + (it->y)* SNAKE_WIDTH);
		pre = it;
	}

	// 绘制蛇尾
	const Segment& tail = m_snakelist.back();
	map.drawBlock(tail.x, tail.y);

	IMAGE tail_img;//尾巴的方向取前一个节点的方向
	it = std::prev(it, 1);
	switch (it->dir) {
	case DIR_UP: tail_img = tailDown; break;
	case DIR_RIGHT: tail_img = tailLeft; break;
	case DIR_DOWN: tail_img = tailUp; break;
	case DIR_LEFT: tail_img = tailRight; break;
	}
	drawAlpha(&tail_img, map.m_x + tail.x * SNAKE_WIDTH, map.m_y + tail.y * SNAKE_WIDTH);

	//清除尾巴
	map.drawBlock(m_tail.x, m_tail.y);
}

Snake::~Snake()
{
}