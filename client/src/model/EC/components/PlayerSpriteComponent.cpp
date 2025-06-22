#include "../../../../../client/include/model/EC/components/PlayerSpriteComponent.h"


void PlayerSpriteComponent::init(const SpriteType type, const PlayerState state,
                                 const TypeWeapon weapon_type) {
    SpriteComponent::init(type);
    this->state = state;
    update(state, weapon_type);
}

void PlayerSpriteComponent::update(const PlayerState state, const TypeWeapon weapon_type) {
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

PlayerState PlayerSpriteComponent::getState() const {
    return state;
}


// void PlayerSpriteComponent::setTypeWeaponEquipped(const TypeWeapon weapon_type) {
//     int row = 0;
//     switch (weapon_type) {
//         case TypeWeapon::Knife:
//             row = 1;
//             break;
//         case TypeWeapon::Primary:
//             row = 2;
//             break;
//         case TypeWeapon::Bomb:
//         case TypeWeapon::Secondary:
//             row = 3;
//             break;
//         default:
//             break;
//     }
//
//     setFrame(row, 0);
// }
