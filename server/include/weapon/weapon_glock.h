#ifndef WEAPON_GLOCK_H
#define WEAPON_GLOCK_H

#include <string>
#include <vector>

#include "../../../common/types.h"

#include "fire_weapon.h"

class WeaponGlock: public FireWeapon {
public:
    WeaponGlock();

    Weapon getWeaponType() const override;
    WeaponInfo generateWeaponInfo(const WeaponState& state) override;
    std::vector<Projectile> shoot(float posX, float posY, float dirX, float dirY,
                                  const std::string& shooter, uint64_t currentTimeMs) override;
};

#endif
