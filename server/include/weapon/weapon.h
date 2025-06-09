#ifndef WEAPON_H
#define WEAPON_H

#include <cstdint>
#include "common/types.h"

class Weapon_ {
protected:
    int damage;

public:
    explicit Weapon_(const int damage): damage(damage) {}
    virtual ~Weapon_() = default;

    virtual int use(uint64_t currentTimeMs) = 0;

    virtual bool canShoot(uint64_t currentTimeMs) const = 0;

    virtual uint64_t getCooldownMs() const = 0;

    virtual int getBullets() const { return 0;};

    virtual Weapon getWeaponType() const = 0;

    virtual int getDamage() const { return damage; }
};

#endif
