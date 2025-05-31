#ifndef MAPINFO_H
#define MAPINFO_H

#include <vector>

#include "client/include/model/utils/SpriteType.h"

struct MapInfo {
    const std::vector<std::vector<int>> tileMap;
    const SpriteType tileSetType;
    const int numTilesInX;
    const int numTilesInY;

    MapInfo(const std::vector<std::vector<int>>& tileMap, const SpriteType tileSetType,
            const int numTilesInX, const int numTilesInY):
            tileMap(tileMap),
            tileSetType(tileSetType),
            numTilesInX(numTilesInX),
            numTilesInY(numTilesInY) {}
};

#endif  // MAPINFO_H
