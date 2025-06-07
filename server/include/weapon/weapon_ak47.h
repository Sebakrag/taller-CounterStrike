#ifndef WEAPON_AK47_H
#define WEAPON_AK47_H

#include "fire_weapon.h"
#include "common/types.h"

class WeaponAk47: public FireWeapon {
public:
    WeaponAk47();

    Weapon getWeaponType() const override;
};

#endif
