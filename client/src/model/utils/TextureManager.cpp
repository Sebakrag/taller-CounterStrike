#include "client/include/model/utils/TextureManager.h"

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>

#include "client/client_constants.h"

using SDL2pp::Surface;

Renderer* TextureManager::renderer = nullptr;
std::unordered_map<SpriteType, std::shared_ptr<Texture>> TextureManager::textures;

void TextureManager::init(Renderer& ren) {
    renderer = &ren;

    // Players textures
    constexpr Color playerColorKey = {255, 0, 255, SDL_ALPHA_OPAQUE};
    loadTexture(SpriteType::RANDOM_PLAYER, RANDOM_PLAYER_IMG, playerColorKey);
    loadTexture(SpriteType::PHEONIX, PHOENIX_IMG, playerColorKey);
    loadTexture(SpriteType::L337_KREW, L377_KREW_IMG, playerColorKey);
    loadTexture(SpriteType::ARTIC_AVENGER, ARTIC_AVENGER_IMG, playerColorKey);
    loadTexture(SpriteType::GUERRILLA, GUERRILLA_IMG, playerColorKey);
    loadTexture(SpriteType::SEAL_FORCE, SEAL_FORCE_IMG, playerColorKey);
    loadTexture(SpriteType::GERMAN_GSG_9, GERMAN_GSG_9_IMG, playerColorKey);
    loadTexture(SpriteType::UK_SAS, UK_SAS_IMG, playerColorKey);
    loadTexture(SpriteType::FRENCH_GIGN, FRENCH_GIGN_IMG, playerColorKey);

    // Uso version con blending
    loadTexture(SpriteType::BOMB, BOMB_IMG);
    loadTexture(SpriteType::BOMB_PLANTED, BOMB_PLANTED_IMG_2);
    loadTexture(SpriteType::AK47, AK47_IMG_2);
    loadTexture(SpriteType::AWP, AWP_IMG_2);
    loadTexture(SpriteType::GLOCK, GLOCK_IMG_2);
    loadTexture(SpriteType::KNIFE, KNIFE_IMG_2);
    loadTexture(SpriteType::M3, M3_IMG_2);
    loadTexture(SpriteType::BULLET, BULLET_IMG_2);

    // Map textures
    loadTexture(SpriteType::TRAINING_MAP, TRAINING_TILE_SET_IMG);
    loadTexture(SpriteType::DESERT_MAP, DESERT_TILE_SET_IMG);
    loadTexture(SpriteType::AZTEC_MAP, AZTEC_TILE_SET_IMG);

    // HUD textures:
    constexpr Color hudColorKey = {0, 0, 0, SDL_ALPHA_OPAQUE};
    loadTexture(SpriteType::HUD_NUMBERS, HUD_NUMS, hudColorKey);
    loadTexture(SpriteType::HUD_SYMBOLS, HUD_SYMB, hudColorKey);
}

void TextureManager::loadTexture(const SpriteType type, const std::string& path) {
    Surface surface(path);
    textures[type] = std::make_shared<Texture>(*renderer, surface);
}

void TextureManager::loadTexture(const SpriteType type, const std::string& path,
                                 const Color& colorKey) {
    Surface surface(path);
    const Uint32 mappedKey = SDL_MapRGB(surface.Get()->format, colorKey.GetRed(),
                                        colorKey.GetGreen(), colorKey.GetBlue());
    surface.SetColorKey(SDL_TRUE, mappedKey);
    textures[type] = std::make_shared<Texture>(*renderer, surface);
}

std::shared_ptr<Texture> TextureManager::getTexture(const SpriteType type) {
    return textures[type];
}

std::shared_ptr<Texture> TextureManager::getTextureMap(const TypeTileMap& typeTileMap) {
    if (typeTileMap == TypeTileMap::Desert) {
        return textures[SpriteType::DESERT_MAP];
    } else if (typeTileMap == TypeTileMap::Aztec) {
        return textures[SpriteType::AZTEC_MAP];
    } else {
        return textures[SpriteType::TRAINING_MAP];
    }
}

std::unique_ptr<Texture> TextureManager::createTextTexture(const std::string& text, Font& font,
                                                           const Color& color) {
    if (!renderer)
        throw std::runtime_error("Renderer not initialized in TextureManager!");

    Surface surface = font.RenderText_Blended(text, color);
    return std::make_unique<Texture>(*renderer, surface);
}
