#include "client/include/model/EC/components/SpriteComponent.h"

#include "client/include/model/utils/SpriteMetadataProvider.h"
#include "client/include/model/utils/TextureManager.h"

void SpriteComponent::init(const SpriteType type) {
    setTexture(type);

    // TODO: cambiar SpriteMetadataProvider: Me gustaria hacer el calculo con el tamanio en pixels
    // Directamente podriamos hacer:
    // frameWidth = SPRITE_SIZE;  // con SPRITE_SIZE 32 (en pixels)
    // frameHeight = SPRITE_SIZE;
    // (32x32)
    const SpriteSheetLayout layout = SpriteMetadataProvider::getLayout(type);

    frameWidth = (texture->GetWidth() / layout.numSpritesAlongWidth);
    frameHeight = (texture->GetHeight() / layout.numSpritesAlongHeight);
    spriteRect.SetW(frameWidth);
    spriteRect.SetH(frameHeight);
    spriteRect.SetX(0);
    spriteRect.SetY(0);
}

void SpriteComponent::setFrame(const int row, const int col) {
    spriteRect.SetX(col * frameWidth);
    spriteRect.SetY(row * frameHeight);
}

Rect SpriteComponent::getSpriteRect() const { return spriteRect; }

std::shared_ptr<Texture> SpriteComponent::getTexture() const { return texture; }

int SpriteComponent::getWidth() const { return frameWidth; }

int SpriteComponent::getHeight() const { return frameHeight; }

void SpriteComponent::setTexture(const SpriteType type) {
    texture = TextureManager::getTexture(type);
}
