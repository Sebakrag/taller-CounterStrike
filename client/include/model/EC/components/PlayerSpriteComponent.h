#ifndef PLAYERSPRITECOMPONENT_H
#define PLAYERSPRITECOMPONENT_H

#include "../../../../../common/types.h"

#include "SpriteComponent.h"

class PlayerSpriteComponent: public SpriteComponent {
private:
    PlayerState state = PlayerState::Idle;
    SpriteType skin = SpriteType::RANDOM_PLAYER;

public:
    void init(SpriteType initialSkin, PlayerState initialState, TypeWeapon initialWeaponType);
    void update(SpriteType newSkin, PlayerState state, TypeWeapon weapon_type);
    PlayerState getState() const;
};

#endif  // PLAYERSPRITECOMPONENT_H
