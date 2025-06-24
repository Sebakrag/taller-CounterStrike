#ifndef WEAPON_INFO_H_
#define WEAPON_INFO_H_

#include <cstdint>
#include <vector>

#include "../protocol.h"
#include "../types.h"

#define SIZE_ITEM_INFO 16

struct WeaponInfo {
    ServerEntityID server_entt_id;
    Weapon weapon;
    WeaponState state;
    int ammo = 0;  // si es un arma, tendrá sus balas
    float pos_x;
    float pos_y;

    WeaponInfo() = default;
    WeaponInfo(ServerEntityID server_entt_id, Weapon weapon, WeaponState state, int ammo,
               float pos_x, float pos_y);

    WeaponInfo(const WeaponInfo& other) = default;
    WeaponInfo& operator=(const WeaponInfo& other) = default;

    explicit WeaponInfo(const std::vector<uint8_t>& bytes);

    std::vector<uint8_t> toBytes() const;

    SpriteType getSpriteType();
};

#endif  // WEAPON_INFO_H_
