#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <memory>

#include <SDL2pp/SDL2pp.hh>

#include "client/include/model/utils/SpriteType.h"

using SDL2pp::Rect;
using SDL2pp::Texture;

class SpriteComponent {
private:
    std::shared_ptr<Texture> texture;
    Rect spriteRect;
    Rect onMapRect;

public:
    SpriteComponent() = default;

    void init(SpriteType type, float x, float y);

    void setTexture(SpriteType type);

    void update(const float x, const float y);
};

#endif  // SPRITECOMPONENT_H
