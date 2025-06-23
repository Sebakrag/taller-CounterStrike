#include "../../../../../client/include/model/EC/components/SpriteComponent.h"

#include "../../../../../client/client_constants.h"
#include "../../../../../client/include/model/utils/SpriteMetadataProvider.h"
#include "../../../../../client/include/model/utils/TextureManager.h"

void SpriteComponent::init(const SpriteType type) {
    setTexture(type);

    frameWidth = SPRITE_SIZE;
    frameHeight = SPRITE_SIZE;
    spriteRect.SetW(frameWidth);
    spriteRect.SetH(frameHeight);
    spriteRect.SetX(0);
    spriteRect.SetY(0);
    std::cout << "SriteComponent instanciadooo!" << std::endl;
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
