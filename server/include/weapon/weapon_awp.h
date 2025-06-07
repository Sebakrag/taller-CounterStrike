#ifndef WEAPON_AWP_H
#define WEAPON_AWP_H

#include "fire_weapon.h"
#include "common/types.h"

class WeaponAwp: public FireWeapon {
public:
    WeaponAwp();

    Weapon getWeaponType() const override;
};

#endif
