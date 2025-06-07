#ifndef WEAPON_KNIFE_H
#define WEAPON_KNIFE_H

#include "server/include/weapon/weapon.h"

class WeaponKnife : public Weapon_{
public:
    WeaponKnife();
    ~WeaponKnife() override = default;

    int use(uint64_t currentTimeMs) override;
    bool canShoot(uint64_t currentTimeMs) const override;
    uint64_t getCooldownMs() const override;
    Weapon getWeaponType() const override;
};

#endif
