#include "../include/map.h"

#include <iostream>


Map::Map() : tileMap(TileMap::getCurrentLevel()) {}

bool Map::isWalkable(int x, int y) {
    int tileX = x / TILE_SIZE;
    int tileY = y / TILE_SIZE;

    if (tileX < 0 || tileX >= tileMap.getColCount() ||
        tileY < 0 || tileY >= tileMap.getRowCount()) {
        return false;
    }

    return !tileMap.getTile(tileY, tileX).isSolid();
}



