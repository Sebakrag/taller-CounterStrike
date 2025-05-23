#ifndef MAP_H
#define MAP_H

#include <string>

#include <SDL2pp/SDL2pp.hh>

using SDL2pp::Rect;
using SDL2pp::Renderer;
using SDL2pp::Texture;
using SDL2pp::Window;

class Map {
private:
    Renderer& renderer;
    Texture texture;
    Rect srcRect, dstRect;
    int cnt;

public:
    Map(Renderer& ren, const std::string& map_scene, const Window& win);

    void update();
    void render();

    Map(Map&& other) noexcept = default;
    Map& operator=(Map&& other) noexcept = delete;

    Map(const Map& other) = delete;
    Map& operator=(const Map& other) = delete;
};

#endif  // MAP_H
