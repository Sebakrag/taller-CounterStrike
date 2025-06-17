#include "client/include/model/EC/components/PlayerSpriteComponent.h"


void PlayerSpriteComponent::init(const SpriteType type, const TypeWeapon weapon_type) {
    SpriteComponent::init(type);
    setTypeWeaponEquipped(weapon_type);
}

void PlayerSpriteComponent::setTypeWeaponEquipped(const TypeWeapon weapon_type) {
    int row = 0;
    switch (weapon_type) {
        case TypeWeapon::Knife:
            row = 0;
            break;
        case TypeWeapon::Primary:
            row = 1;
            break;
        case TypeWeapon::Bomb:
        case TypeWeapon::Secondary:
            row = 2;
            break;
        default:
            break;
    }

    setFrame(row, 0);
}
