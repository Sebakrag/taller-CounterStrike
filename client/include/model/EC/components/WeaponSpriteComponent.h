#ifndef WEAPONSPRITECOMPONENT_H
#define WEAPONSPRITECOMPONENT_H

#include "common/types.h"

#include "SpriteComponent.h"

class WeaponSpriteComponent: public SpriteComponent {
private:
    WeaponState state;

    void setFrameForState(WeaponState state);

public:
    WeaponSpriteComponent(): state(WeaponState::HIDDEN) {}

    void init(SpriteType type, WeaponState initialState);
    void setState(WeaponState newState);
    WeaponState getState() const;
};


#endif  // WEAPONSPRITECOMPONENT_H
