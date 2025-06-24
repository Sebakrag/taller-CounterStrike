#ifndef SYMBOLSRENDERER_H
#define SYMBOLSRENDERER_H

#include <memory>

#include "SDL2pp/SDL2pp.hh"

#include "SpriteType.h"

#define HUD_SYMBOL_WIDTH 64
#define HUD_SYMBOL_HEIGHT 64

using SDL2pp::Rect;
using SDL2pp::Texture;

class Graphics;

// Para que funcione, es importante que el orden en que aparezcan los simbolos en el sprite
// coincida con el orden de los simbolos definidos en el enum.
enum class HUDSymbolType : unsigned char { HEALTH, CLOCK, MONEY, BOMB, NONE };

class SymbolsRenderer {
private:
    std::shared_ptr<Texture> texture;
    const int symbolWidth = HUD_SYMBOL_WIDTH;
    const int symbolHeight = HUD_SYMBOL_HEIGHT;

public:
    explicit SymbolsRenderer(SpriteType type);

    void renderSymbol(Graphics& graphics, HUDSymbolType symbol, int x, int y,
                      float scale = 1) const;
    int getSymbolW() const;
    int getSymbolH() const;
};

#endif  // SYMBOLSRENDERER_H
