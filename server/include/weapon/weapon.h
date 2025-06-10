#ifndef WEAPON_H
#define WEAPON_H

#include <cstdint>
#include "common/types.h"
#include "server/include/weapon/projectile.h"

class Weapon_ {
protected:
    int damage;

public:
    explicit Weapon_(const int damage): damage(damage) {}
    virtual ~Weapon_() = default;

    virtual bool canShoot(uint64_t currentTimeMs) const = 0;

    virtual uint64_t getCooldownMs() const = 0;

    virtual int getBullets() const { return 0;};

    virtual Weapon getWeaponType() const = 0;

    virtual int getDamage() const { return damage; }

    virtual std::vector<Projectile> shoot(float posX, float posY, float dirX, float dirY, const std::string& shooter, uint64_t currentTimeMs) = 0;
};

#endif
