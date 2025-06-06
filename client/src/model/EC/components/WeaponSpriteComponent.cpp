#include "client/include/model/EC/components/WeaponSpriteComponent.h"


void WeaponSpriteComponent::init(const SpriteType type, const WeaponState initialState) {
    SpriteComponent::init(type);
    state = initialState;
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
    std::cout << "[DEBUG] setFrameForState recibido: " << static_cast<int>(state) << std::endl;

    switch (state) {
        case WeaponState::DROPPED:
            std::cout << "Tiene el estado DROPPED: " << static_cast<int>(state) << std::endl;
            row = 0;
            break;
        case WeaponState::EQUIPPED:
            std::cout << "Tiene el estado EQUIPPED: " << static_cast<int>(state) << std::endl;
            row = 1;
            break;
        case WeaponState::HIDDEN:
            std::cout << "Tiene el estado HIDDEN: " << static_cast<int>(state) << std::endl;
            // No importa el rect, no se renderiza
            return;
        default:
            std::cout << "cualquier estado che: " << static_cast<int>(state) << std::endl;
            return;
    }

    setFrame(row, 0);
}
