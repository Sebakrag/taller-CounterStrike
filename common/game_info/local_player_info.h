#ifndef HUD_INFO_H_
#define HUD_INFO_H_


#include <cstdint>
#include <vector>

#include "../../client/include/model/utils/SpriteType.h"
#include "../types.h"

#define SIZE_LOCAL_PLAYER_INFO 29

struct LocalPlayerInfo {
    ServerEntityID server_entt_id;
    Team team;
    PlayerSkin skin;
    PlayerState state;
    Vec2D position;
    float angle_direction;
    TypeWeapon weapon_type;
    int health;
    int money;
    int ammo_weapon;  // TODO: AmmoInfo
    ServerEntityID equipped_weapon_id;

    LocalPlayerInfo() {}

    LocalPlayerInfo(ServerEntityID server_entt_id, Team team, PlayerSkin skin,
                    const Vec2D& position, float angle_direction, TypeWeapon weapon_type,
                    int health, int money, int ammo, ServerEntityID equipped_weapon_id);

    LocalPlayerInfo(const LocalPlayerInfo& other) = default;
    LocalPlayerInfo& operator=(const LocalPlayerInfo& other) = default;

    SpriteType generateSpriteType() const;

    void print() const;

    explicit LocalPlayerInfo(const std::vector<uint8_t>& bytes);

    std::vector<uint8_t> toBytes() const;
};

#endif
