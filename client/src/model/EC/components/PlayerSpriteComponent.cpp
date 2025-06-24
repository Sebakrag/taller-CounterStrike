#include "../../../../../client/include/model/EC/components/PlayerSpriteComponent.h"

void PlayerSpriteComponent::init(const SpriteType initialSkin, const PlayerState initialState,
                                 const TypeWeapon initialWeaponType) {
    SpriteComponent::init(initialSkin);
    this->state = initialState;
    update(initialSkin, initialState, initialWeaponType);
}

void PlayerSpriteComponent::update(const SpriteType newSkin, const PlayerState state,
                                   const TypeWeapon weapon_type) {
    if (this->skin != newSkin)
        SpriteComponent::setTexture(newSkin);

    this->state = state;
    int row = 0;
    if (state != PlayerState::Dead) {
        switch (weapon_type) {
            case TypeWeapon::Knife:
                row = 1;
                break;
            case TypeWeapon::Primary:
                row = 2;
                break;
            case TypeWeapon::Bomb:
            case TypeWeapon::Secondary:
                row = 3;
                break;
            default:
                break;
        }
    }

    setFrame(row, 0);
}

PlayerState PlayerSpriteComponent::getState() const { return state; }
