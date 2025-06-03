#include "../include/map.h"

#include <iostream>


Map::Map() : tileMap(TileMap::getLevelDemo()) {}

bool Map::isWalkable(int x, int y) {
    if (x < 0 || y < 0 || x >= 640 || y >= 400)
        return false;

    return true;
}

/*Map::Map() : tileMap(TileMap::getLevelDemo()) {}

bool Map::isWalkable(float x, float y) {
    const int TILE_SIZE = 64;

    int tileX = static_cast<int>(x) / TILE_SIZE;
    int tileY = static_cast<int>(y) / TILE_SIZE;

    if (tileX < 0 || tileY < 0 ||
        tileX >= 640 || tileY >= 400) {
        return false;
    }

    return !tileMap.getTile(tileY, tileX).isSolid();
}*/

