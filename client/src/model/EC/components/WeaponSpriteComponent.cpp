#include "../../../../../client/include/model/EC/components/WeaponSpriteComponent.h"

#include "../../../../../client/client_constants.h"

void WeaponSpriteComponent::init(const SpriteType type, const WeaponState initialState) {
    SpriteComponent::init(type);
    state = initialState;
    if (type == SpriteType::AK47 || type == SpriteType::AWP) {  // tienen sprites mas anchos
        frameWidth = SPRITE_LONG_WIDTH;
    }
    setFrameForState(state);
}

void WeaponSpriteComponent::setState(const WeaponState newState) {
    if (newState == state)
        return;

    state = newState;
    setFrameForState(state);
}

WeaponState WeaponSpriteComponent::getState() const { return state; }

Vec2D WeaponSpriteComponent::getRenderOffset() const { return renderOffset; }


void WeaponSpriteComponent::setFrameForState(const WeaponState state) {
    int row = 0;

    switch (state) {
        case WeaponState::DROPPED:
            row = 0;
            break;
        case WeaponState::EQUIPPED:
            row = 1;
            break;
        case WeaponState::HIDDEN:
            // No importa el rect, no se renderiza
            return;
        default:
            return;
    }

    setFrame(row, 0);
}
