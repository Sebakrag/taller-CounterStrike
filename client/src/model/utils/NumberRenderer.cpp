#include "client/include/model/utils/NumberRenderer.h"

#include "client/include/model/Graphics.h"
#include "client/include/model/utils/TextureManager.h"

#define SEPARATOR_WIDTH 15  // in pixels

NumberRenderer::NumberRenderer(const SpriteType type, const int w, const int h):
        texture(TextureManager::getTexture(type)), destRectW(w), destRectH(h) {
    const std::string chars = "0123456789:|";
    for (size_t i = 0; i < chars.size(); ++i) {
        const int srcW = (chars[i] == ':' || chars[i] == '|') ? SEPARATOR_WIDTH : w;
        charMap[chars[i]] = Rect(static_cast<int>(i * w), 0, srcW, h);
    }
}

void NumberRenderer::renderFromText(Graphics& graphics, const std::string& text, const int x,
                                    const int y, SDL_Color color) {
    int cursorX = x;
    texture->SetColorMod(color.r, color.g, color.b);

    for (const char c: text) {
        Rect srcRect = getSourceRect(c);
        int scaledW = static_cast<int>((destRectW * srcRect.w) / NUMBER_WIDTH);
        int scaledH = static_cast<int>((destRectH * srcRect.h) / CHAR_HEIGHT);

        Rect destRect(cursorX, y, scaledW, scaledH);
        graphics.draw(*texture, srcRect, destRect);

        cursorX += scaledW;
    }

    // Restaurar a blanco
    texture->SetColorMod(255, 255, 255);
}

Rect NumberRenderer::measureText(const std::string& text) {
    int totalW = 0;
    for (const char c: text) {
        Rect src = getSourceRect(c);
        totalW += static_cast<int>((destRectW * src.w) / NUMBER_WIDTH);
    }
    return {0, 0, totalW, destRectH};  // no importa x, y
}

void NumberRenderer::setRenderSize(const int w, const int h) {
    destRectW = w;
    destRectH = h;
}

Rect NumberRenderer::getSourceRect(const char c) {
    const auto it = charMap.find(c);
    if (it == charMap.end()) {
        throw std::runtime_error(std::string("Character not supported: ") + c);
    }
    return it->second;
}
