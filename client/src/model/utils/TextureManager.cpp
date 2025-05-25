#include "client/include/model/utils/TextureManager.h"

#include "client/client_constants.h"

using SDL2pp::Surface;

std::unordered_map<SpriteType, std::shared_ptr<Texture>> TextureManager::textures;

void TextureManager::init(Renderer& ren) {
    const Color playerColorKey = {0, 0, 0, 255};
    loadTexture(ren, SpriteType::PHEONIX, PHOENIX_IMG, playerColorKey);
    loadTexture(ren, SpriteType::L337_KREW, L377_KREW_IMG, playerColorKey);
    loadTexture(ren, SpriteType::ARTIC_AVENGER, ARTIC_AVENGER_IMG, playerColorKey);
    loadTexture(ren, SpriteType::GUERRILLA, GUERRILLA_IMG, playerColorKey);
    loadTexture(ren, SpriteType::SEAL_FORCE, SEAL_FORCE_IMG, playerColorKey);
    loadTexture(ren, SpriteType::GERMAN_GSG_9, GERMAN_GSG_9_IMG, playerColorKey);
    loadTexture(ren, SpriteType::UK_SAS, UK_SAS_IMG, playerColorKey);
    loadTexture(ren, SpriteType::FRENCH_GIGN, FRENCH_GIGN_IMG, playerColorKey);
}

void TextureManager::loadTexture(Renderer& ren, const SpriteType type, const std::string& path,
                                 const Color& colorKey) {
    Surface surface(path);
    surface.SetColorAndAlphaMod(colorKey);
    textures[type] = std::make_shared<Texture>(ren, surface);
}

std::shared_ptr<Texture> TextureManager::getTexture(SpriteType type) { return textures[type]; }
