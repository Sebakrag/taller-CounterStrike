#include "client/include/model/utils/SymbolsRenderer.h"

#include "client/include/model/Graphics.h"
#include "client/include/model/utils/TextureManager.h"

SymbolsRenderer::SymbolsRenderer(const SpriteType type):
        texture(TextureManager::getTexture(type)) {}


void SymbolsRenderer::renderSymbol(Graphics& graphics, const HUDSymbolType symbol, const int x,
                                   const int y, const float scale) const {
    const auto index = static_cast<int>(symbol);
    Rect srcRect(index * symbolWidth, 0, symbolWidth, symbolHeight);
    Rect destRect(x, y, static_cast<int>(static_cast<float>(symbolWidth) * scale),
                  static_cast<int>(static_cast<float>(symbolHeight) * scale));

    graphics.draw(*texture, srcRect, destRect);
}

int SymbolsRenderer::getSymbolW() const { return symbolWidth; }

int SymbolsRenderer::getSymbolH() const { return symbolHeight; }
