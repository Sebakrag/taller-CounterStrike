#ifndef WEAPON_GLOCK_H
#define WEAPON_GLOCK_H

#include "fire_weapon.h"
#include "common/types.h"

class WeaponGlock: public FireWeapon {
public:
    WeaponGlock();

    Weapon getWeaponType() const override;
    std::vector<Projectile> shoot(float posX, float posY, float dirX, float dirY, const std::string &shooter, uint64_t currentTimeMs) override;
};

#endif
