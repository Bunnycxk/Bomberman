#pragma once
#include "../common/common.h"

#define MAP_CELL_SIZE 64
#define MAP_ROW 8
#define MAP_COLUMN 12

#define MAP_LEFT 	224
#define MAP_TOP 	44
#define MAP_RIGHT   992
#define MAP_BOTTOM 	556

#define get_map_x(x) (((x) - MAP_LEFT) / MAP_CELL_SIZE)
#define get_map_y(y) (((y) - MAP_TOP) / MAP_CELL_SIZE)

enum map_type {MAP_EMPTY, MAP_DESTRUCTIBLE, MAP_INDESTRUCTIBLE, MAP_PROPS, MAP_BOMB};


class Map
{
private:
    /* data */    
public:
    uint cell[MAP_ROW][MAP_COLUMN];

    Map(const char *filename);
    ~Map();
};


