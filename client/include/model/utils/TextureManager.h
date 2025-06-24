#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <memory>
#include <string>
#include <unordered_map>

#include <SDL2pp/SDL2pp.hh>

#include "common/types.h"

#include "SpriteType.h"

using SDL2pp::Color;
using SDL2pp::Font;
using SDL2pp::Renderer;
using SDL2pp::Texture;

class TextureManager {
private:
    static Renderer* renderer;
    static std::unordered_map<SpriteType, std::shared_ptr<Texture>> textures;

    static void loadTexture(SpriteType type, const std::string& path);
    static void loadTexture(SpriteType type, const std::string& path, const Color& colorKey);

public:
    TextureManager() = delete;  // cannot be instantiated

    static void init(Renderer& ren);
    static std::shared_ptr<Texture> getTexture(SpriteType type);
    static std::shared_ptr<Texture> getTextureMap(const TypeTileMap& typeTileMap);
    static std::unique_ptr<Texture> createTextTexture(const std::string& text, Font& font,
                                                      const Color& color);
};

#endif  // TEXTUREMANAGER_H
