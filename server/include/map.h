#ifndef MAP_H
#define MAP_H

#include <vector>

#include "../../common/tile_map.h"

class Map {
private:
    TileMap tileMap;
    const int TILE_SIZE = 32;

public:
    explicit Map(const TileMap& tilemap);
    bool isWalkable(int x, int y);
    bool isBombZone(int x, int y);
};

#endif
