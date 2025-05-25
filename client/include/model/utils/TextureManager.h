#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <memory>
#include <string>
#include <unordered_map>

#include <SDL2pp/SDL2pp.hh>

#include "SpriteType.h"

using SDL2pp::Color;
using SDL2pp::Renderer;
using SDL2pp::Texture;

class TextureManager {
private:
    static std::unordered_map<SpriteType, std::shared_ptr<Texture>> textures;
    TextureManager() = delete;  // cannot be instantiated

    static void loadTexture(Renderer& ren, SpriteType type, const std::string& path,
                            const Color& colorKey);

public:
    static void init(Renderer& ren);

    static std::shared_ptr<Texture> getTexture(SpriteType type);
};

#endif  // TEXTUREMANAGER_H
