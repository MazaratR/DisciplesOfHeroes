#include "grid_maker.h"

#include <cmath>

void GridMaker::setSizeInPixels(int _width, int _height)
{
    m_width_px = _width;
    m_height_px = _height;

    recalcTileSize();
}

void GridMaker::setTileCount(int _x, int _y)
{
    m_gridBlocks_x = _x;
    m_gridBlocks_y = _y;

    recalcTileSize();
}

Point2D_int GridMaker::getTileIndexFromPixels(int _x, int _y) const
{
    int x = std::floor(_x / m_tileSize_px.x);
    int y = std::floor(_y / m_tileSize_px.y);
    return Point2D_int(x, y);
}

Point2D_int GridMaker::getTileCenterFromPixels(int _x, int _y) const
{
    Point2D_int blockIndex = getTileIndexFromPixels(_x, _y);

    return getTileCenterFromIndex(blockIndex.x, blockIndex.y);
}

Point2D_int GridMaker::getTileLeftTopFromPixels(int _x, int _y) const
{
    Point2D_int blockIndex = getTileIndexFromPixels(_x, _y);

    return getTileLeftTopFromIndex(blockIndex.x, blockIndex.y);
}

Point2D_int GridMaker::getTileLeftBottomFromPixels(int _x, int _y) const
{
    Point2D_int blockIndex = getTileIndexFromPixels(_x, _y);

    return getTileLeftBottomFromIndex(blockIndex.x, blockIndex.y);
}

Point2D_int GridMaker::getTileCenterFromIndex(int _x, int _y) const
{
    int x = _x * m_tileSize_px.x + m_tileSize_px.x / 2;
    int y = _y * m_tileSize_px.y + m_tileSize_px.y / 2;

    return Point2D_int(x, y);
}

Point2D_int GridMaker::getTileLeftTopFromIndex(int _x, int _y) const
{
    int x = _x * m_tileSize_px.x;
    int y = _y * m_tileSize_px.y;

    return Point2D_int(x, y);
}

Point2D_int GridMaker::getTileLeftBottomFromIndex(int _x, int _y) const
{
    int x = _x * m_tileSize_px.x;
    int y = _y * m_tileSize_px.y + m_tileSize_px.y;

    return Point2D_int(x, y);
}

void GridMaker::recalcTileSize()
{
    if (m_width_px && m_height_px && m_gridBlocks_x && m_gridBlocks_y)
    {
        m_tileSize_px.x = std::ceil(m_width_px / m_gridBlocks_x);
        m_tileSize_px.y = std::ceil(m_height_px / m_gridBlocks_y);
    }
}


