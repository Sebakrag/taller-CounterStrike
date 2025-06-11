#ifndef WEAPON_AWP_H
#define WEAPON_AWP_H

#include "fire_weapon.h"
#include "common/types.h"

class WeaponAwp: public FireWeapon {
public:
    WeaponAwp();

    Weapon getWeaponType() const override;
    std::vector<Projectile> shoot(float posX, float posY, float dirX, float dirY, const std::string &shooter, uint64_t currentTimeMs) override;
};

#endif
