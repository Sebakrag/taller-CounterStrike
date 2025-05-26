#ifndef MAPINFO_H
#define MAPINFO_H

#include <vector>

#include "client/include/model/utils/SpriteType.h"

struct MapInfo {
    const std::vector<std::vector<int>> tileMap;
    const SpriteType tileSetType;
    const int width;
    const int height;

    MapInfo(const std::vector<std::vector<int>>& tileMap, const SpriteType tileSetType, const int w,
            const int h):
            tileMap(tileMap), tileSetType(tileSetType), width(w), height(h) {}
};

#endif  // MAPINFO_H
