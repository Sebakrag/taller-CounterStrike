#include "client/include/model/EC/components/SpriteComponent.h"

#include "client/include/model/utils/SpriteMetadataProvider.h"
#include "client/include/model/utils/TextureManager.h"

void SpriteComponent::init(const SpriteType type, const float x, const float y) {
    setTexture(type);

    const SpriteSheetLayout layout = SpriteMetadataProvider::getLayout(type);

    const int spriteWidth = (texture->GetWidth() / layout.numSpritesAlongWidth);
    const int spriteHeight = (texture->GetHeight() / layout.numSpritesAlongHeight);
    spriteRect.SetW(spriteWidth);
    spriteRect.SetH(spriteHeight);
    // Estas posiciones de x e y deberian ser acorde al frame de animacion.
    spriteRect.SetX(0);
    spriteRect.SetY(0);

    onMapRect.SetW(spriteWidth);
    onMapRect.SetH(spriteHeight);
    onMapRect.SetX(static_cast<int>(x));
    onMapRect.SetY(static_cast<int>(y));
}

void SpriteComponent::setTexture(const SpriteType type) {
    texture = TextureManager::getTexture(type);
}

void SpriteComponent::update(const float x, const float y) {
    onMapRect.SetX(static_cast<int>(x));
    onMapRect.SetY(static_cast<int>(y));
}
