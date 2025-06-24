#ifndef NUMBERRENDERER_H
#define NUMBERRENDERER_H

#include <memory>
#include <string>
#include <unordered_map>

#include "SDL2pp/SDL2pp.hh"

#include "SpriteType.h"

#define NUMBER_WIDTH 48  // in pixels
#define CHAR_HEIGHT 66   // in pixels

using SDL2pp::Rect;
using SDL2pp::Texture;

class Graphics;

class NumberRenderer {
private:
    std::shared_ptr<Texture> texture;
    std::unordered_map<char, Rect> charMap;
    int destRectW;
    int destRectH;

    Rect getSourceRect(char c);

public:
    explicit NumberRenderer(SpriteType type, int w = NUMBER_WIDTH, int h = CHAR_HEIGHT);

    void renderFromText(Graphics& graphics, const std::string& text, int x, int y, SDL_Color color = {255, 255, 255, 255});
    Rect measureText(const std::string& text);

    void setRenderSize(int w, int h);
};

#endif  // NUMBERRENDERER_H
