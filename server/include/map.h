#ifndef MAP_H
#define MAP_H

#include <vector>
#include "../../common/tile_map.h"

class Map {
private:
    TileMap tileMap;

public:
    Map();
    bool isWalkable(int x, int y);
};

#endif
