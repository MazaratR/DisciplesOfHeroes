#ifndef GRIDMAKER_H
#define GRIDMAKER_H

#include "point_2d.h"

class GridMaker
{
public:
    static GridMaker& getInstance(){
        static GridMaker instance;
        return instance;
    }

    void setSizeInPixels(int _width, int _height);
    void setTileCount(int _x, int _y);

    Point2D_int getTileIndexFromPixels(int _x, int _y) const;

    Point2D_int getTileCenterFromPixels(int _x, int _y) const;
    Point2D_int getTileLeftTopFromPixels(int _x, int _y) const;
    Point2D_int getTileLeftBottomFromPixels(int _x, int _y) const;

    Point2D_int getTileCenterFromIndex(int _x, int _y) const;
    Point2D_int getTileLeftTopFromIndex(int _x, int _y) const;
    Point2D_int getTileLeftBottomFromIndex(int _x, int _y) const;

    Point2D_int getTileSize() const { return m_tileSize_px; }

private:
    GridMaker() = default;
    GridMaker(const GridMaker&) = delete;

    inline void recalcTileSize();

private:
    int m_width_px = 0;  // in pixels
    int m_height_px = 0; // in pixels

    int m_gridBlocks_x = 0;
    int m_gridBlocks_y = 0;

    Point2D_int m_tileSize_px;
};

#endif // GRIDMAKER_H
