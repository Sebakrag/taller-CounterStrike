#ifndef WEAPON_AK47_H
#define WEAPON_AK47_H

#include "fire_weapon.h"
#include "common/types.h"

class WeaponAk47: public FireWeapon {
public:
    WeaponAk47();

    Weapon getWeaponType() const override;
    std::vector<Projectile> shoot(float posX, float posY, float dirX, float dirY, const std::string &shooter, uint64_t currentTimeMs) override;
};

#endif
