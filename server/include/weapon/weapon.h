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
    uint32_t serverId;

public:
    explicit Weapon_(const int damage): damage(damage), serverId(IdGenerator::getNextId()) {}
    virtual ~Weapon_() = default;

    virtual bool canShoot(uint64_t currentTimeMs) const = 0;

    virtual uint64_t getCooldownMs() const = 0;

    virtual int getBullets() const { return 0; }

    virtual Weapon getWeaponType() const = 0;

    virtual int getDamage() const { return damage; }

    virtual std::vector<Projectile> shoot(float posX, float posY, float dirX, float dirY,
                                          const std::string& shooter, uint64_t currentTimeMs) = 0;

    virtual WeaponInfo generateWeaponInfo(const WeaponState& state) = 0;

    uint32_t getServerId() const { return serverId; }
};

#endif
