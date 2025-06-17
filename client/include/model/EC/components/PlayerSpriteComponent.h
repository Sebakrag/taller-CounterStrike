#ifndef PLAYERSPRITECOMPONENT_H
#define PLAYERSPRITECOMPONENT_H

#include "common/types.h"

#include "SpriteComponent.h"

class PlayerSpriteComponent: public SpriteComponent {
public:
    void init(SpriteType type, TypeWeapon weapon_type);
    void setTypeWeaponEquipped(TypeWeapon weapon_type);
};

#endif  // PLAYERSPRITECOMPONENT_H
