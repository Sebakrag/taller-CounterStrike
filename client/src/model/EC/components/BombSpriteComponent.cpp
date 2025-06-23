#include "client/include/model/EC/components/BombSpriteComponent.h"

void BombSpriteComponent::init(const SpriteType type, const BombState initialState) {
    SpriteComponent::init(type);
    state = initialState;
    setFrameForState(state);
}

void BombSpriteComponent::setState(const BombState newState) {
    if (newState == state)
        return;
    state = newState;
    setFrameForState(state);
}

BombState BombSpriteComponent::getState() const { return state; }

Vec2D BombSpriteComponent::getRenderOffset() const { return renderOffset; }

void BombSpriteComponent::setFrameForState(const BombState state) {
    int row = 0;
    switch (state) {
        case BombState::Dropped:
            row = 0;
            break;
        case BombState::Equipped:
            row = 1;
            break;
        case BombState::Planted:
            row = 2;
            break;
        case BombState::Defused:
            row = 3;
            break;
        case BombState::Exploded:
            row = 4;
            break;               // TODO: Agregar secuencia de explosion.
        case BombState::Hidden:  // No se renderiza
        default:
            return;
    }
    setFrame(row, 0);
}
