#include "Food.h"


Food::Food():x(0),y(0),m_state(false)
{
	srand(static_cast<unsigned>(time(nullptr))); // 初始化随机数种子
    loadImg();
}

void Food::loadImg()
{
	loadimage(&foodImg, L"PNG", _T("apple"), TILE_SIZE, TILE_SIZE, 1);
}

 bool Food::getState()
{
	return m_state;
}

 int Food::getx()
{
    return x; 
}

 int Food::gety()
{
    return y;
}

 void Food::setState(bool new_state)
{
	m_state = new_state;
}

void Food::Generate(Map& map, std::list<Segment> seglist)
{	
    bool validPosition = false;
    while (!validPosition) {
        // 随机生成食物位置
        x = rand() % (map.getWidth());
        y = rand() % (map.getHeight());

        // 检查新位置是否与蛇身体重叠
        validPosition = true;
        for (const auto& segment : seglist) {
            if (segment.x == x && segment.y == y) {
                validPosition = false;
                break;
            }
        }
    }
    m_state = true; // 设置食物为有效状态
}

void Food::drawFood(Map& map)
{
    if (m_state) {
        drawAlpha(&foodImg, map.m_x + x * TILE_SIZE, map.m_y + y * TILE_SIZE);
    }
}

Food::~Food()
{
    ;
}
