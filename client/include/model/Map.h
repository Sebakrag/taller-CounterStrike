#ifndef MAP_H
#define MAP_H

#include <memory>
#include <vector>

#include <SDL2pp/SDL2pp.hh>

#include "../../../common/dtos/MapInfo.h"
#include "../../../common/tile_map.h"

#include "Camera.h"

using SDL2pp::Rect;
using SDL2pp::Texture;

class Graphics;

class Map {
private:
    std::vector<std::vector<Rect>> srcTileMap;
    std::shared_ptr<Texture> tileSetTexture;
    const int widthInTiles;
    const int heightInTiles;

public:
    explicit Map(const TileMap& tileMap);

    void render(Graphics& graphics, const Camera& camera);

    Map(Map&& other) noexcept = default;
    Map& operator=(Map&& other) noexcept = delete;

    Map(const Map& other) = delete;
    Map& operator=(const Map& other) = delete;
};

#endif  // MAP_H
