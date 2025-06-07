#ifndef WEAPON_M3_H
#define WEAPON_M3_H

#include "fire_weapon.h"
#include "common/types.h"

class WeaponM3: public FireWeapon {
public:
    WeaponM3();

    Weapon getWeaponType() const override;
};

#endif
