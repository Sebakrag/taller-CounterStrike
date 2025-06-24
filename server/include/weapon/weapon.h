#ifndef WEAPON_H
#define WEAPON_H

#include <cstdint>
#include <string>
#include <vector>

#include "../../../common/game_info/weapon_info.h"
#include "../../../common/types.h"
#include "../../../server/include/id_generator.h"
#include "../../../server/include/weapon/projectile.h"

class Weapon_ {
protected:
    int damage;
    int maxDamage;
    uint32_t serverId;

public:
    explicit Weapon_(const int damage, const int maxDamage):
            damage(damage), maxDamage(maxDamage), serverId(IdGenerator::getNextId()) {}
    virtual ~Weapon_() = default;

    virtual bool canShoot(double currentTime) const = 0;

    virtual double getCooldownS() const = 0;

    virtual int getBullets() const { return 0; }

    virtual Weapon getWeaponType() const = 0;

    virtual int getDamage() const { return damage; }

    virtual std::vector<Projectile> shoot(float posX, float posY, float dirX, float dirY,
                                          const std::string& shooter, double currentTime) = 0;

    virtual WeaponInfo generateWeaponInfo(const WeaponState& state) = 0;

    uint32_t getServerId() const { return serverId; }

    virtual float getMaxDamage() const = 0;
    virtual int calculateDamage(float distance) const = 0;
};

#endif
