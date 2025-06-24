#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <memory>

#include <SDL2pp/SDL2pp.hh>

#include "../../../../../client/include/model/utils/SpriteType.h"

using SDL2pp::Rect;
using SDL2pp::Texture;

class SpriteComponent {
protected:
    std::shared_ptr<Texture> texture;
    Rect spriteRect;
    int frameWidth = 0;
    int frameHeight = 0;

public:
    SpriteComponent() = default;

    void init(SpriteType type);
    void setFrame(int row, int col);
    Rect getSpriteRect() const;
    std::shared_ptr<Texture> getTexture() const;
    int getWidth() const;
    int getHeight() const;
    void setTexture(SpriteType type);
};

#endif  // SPRITECOMPONENT_H
