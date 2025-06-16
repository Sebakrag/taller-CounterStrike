#ifndef WEAPON_AWP_H
#define WEAPON_AWP_H

#include <string>
#include <vector>

#include "../../../common/types.h"

#include "fire_weapon.h"

class WeaponAwp: public FireWeapon {
public:
    WeaponAwp();

    Weapon getWeaponType() const override;
    WeaponInfo generateWeaponInfo(const WeaponState& state) override;
    std::vector<Projectile> shoot(float posX, float posY, float dirX, float dirY,
                                  const std::string& shooter, uint64_t currentTimeMs) override;
};

#endif
