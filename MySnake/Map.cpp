#include "Map.h"

Map::Map(int x , int y , int w, int h, COLORREF color1, COLORREF color2):m_x(x),m_y(y),
    m_width(w), m_height(h),m_color1(color1),m_color2(color2)
{
	;
}

 int Map::getWidth()
{
    return m_width;
}

 int Map::getHeight()
{
    return m_height;
}

void Map::drawALLMap()
{
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            COLORREF color = (x%2 == y%2) ? m_color1 : m_color2;
            setfillcolor(color);
            solidrectangle(m_x+x* TILE_SIZE, m_y+y* TILE_SIZE, m_x+(x +1)* TILE_SIZE, m_y+(y + 1) * TILE_SIZE );
        }
    }
}

void Map::drawBlock(int x, int y)
{
    COLORREF color = (x% 2 == y% 2) ? m_color1 : m_color2;
    setfillcolor(color);
    solidrectangle(m_x + x * TILE_SIZE, m_y + y * TILE_SIZE, m_x + (x + 1) * TILE_SIZE, m_y + (y + 1) * TILE_SIZE);
}

Map::~Map()
{
    ;
}