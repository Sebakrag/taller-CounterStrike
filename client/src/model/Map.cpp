#include "client/include/model/Map.h"

using SDL2pp::Optional;


Map::Map(Renderer& ren, const std::string& map_scene, const Window& win):
        renderer(ren),
        texture(renderer, map_scene),
        srcRect(0, 0, win.GetDrawableWidth(), win.GetDrawableHeight()),
        dstRect(0, 0, win.GetDrawableWidth(), win.GetDrawableHeight()),
        cnt(0) {}

void Map::update() {
    srcRect.SetX(0);
    srcRect.SetY(cnt);

    cnt++;
}

void Map::render() { renderer.Copy(texture, Optional{srcRect}, Optional{dstRect}); }
