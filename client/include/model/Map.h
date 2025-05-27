#ifndef MAP_H
#define MAP_H

#include <memory>
#include <vector>

#include <SDL2pp/SDL2pp.hh>

#include "client/dtos/MapInfo.h"
#include "utils/Vec2D.h"

using SDL2pp::Rect;
using SDL2pp::Renderer;
using SDL2pp::Texture;

class Map {
private:
    std::vector<std::vector<Rect>> srcTileMap;
    std::shared_ptr<Texture> tileSetTexture;
    const int width;
    const int height;
    const int viewportW;  // ancho del area visible (camara)
    const int viewportH;  // alto del area visible (camara)
    Rect onMapRect;

public:
    explicit Map(const MapInfo& mapInfo);

    void render(Renderer& ren, const Vec2D& posLocalPlayer);
    void render(Renderer& ren);

    Map(Map&& other) noexcept = default;
    Map& operator=(Map&& other) noexcept = delete;

    Map(const Map& other) = delete;
    Map& operator=(const Map& other) = delete;
};

#endif  // MAP_H
