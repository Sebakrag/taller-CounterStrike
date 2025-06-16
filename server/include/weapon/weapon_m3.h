#ifndef WEAPON_M3_H
#define WEAPON_M3_H

#include "fire_weapon.h"
#include "common/types.h"

class WeaponM3: public FireWeapon {
public:
    WeaponM3();

    Weapon getWeaponType() const override;
    std::vector<Projectile> shoot(float posX, float posY, float dirX, float dirY, const std::string &shooter, uint64_t currentTimeMs) override;
};

#endif
