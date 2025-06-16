#ifndef WEAPON_KNIFE_H
#define WEAPON_KNIFE_H

#include <string>
#include <vector>

#include "../../../server/include/weapon/weapon.h"

class WeaponKnife: public Weapon_ {
public:
    WeaponKnife();
    ~WeaponKnife() override = default;

    bool canShoot(uint64_t currentTimeMs) const override;
    uint64_t getCooldownMs() const override;
    Weapon getWeaponType() const override;
    WeaponInfo generateWeaponInfo(const WeaponState& state) override;
    std::vector<Projectile> shoot(float posX, float posY, float dirX, float dirY,
                                  const std::string& shooter, uint64_t currentTimeMs) override;
};

#endif
