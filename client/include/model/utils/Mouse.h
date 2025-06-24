#ifndef MOUSE_H
#define MOUSE_H

#include <memory>

#include <SDL2pp/SDL2pp.hh>

#include "model/utils/TextureManager.h"

using SDL2pp::Rect;
using SDL2pp::Texture;

class Graphics;

class Mouse {
private:
    std::shared_ptr<Texture> texture = TextureManager::getTexture(SpriteType::MOUSE_POINTER);
    Rect srcRect;
    Rect onScreenRect;
    Rect pointer;

public:
    Mouse();

    void update();
    void render(Graphics& graphics);
};

#endif  // MOUSE_H
