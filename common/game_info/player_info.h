#ifndef PLAYER_INFO_H_
#define PLAYER_INFO_H_

#include <cstdint>
#include <string>
#include <vector>

#include "../../client/include/model/utils/SpriteType.h"
#include "../types.h"

struct PlayerInfo {
    ServerEntityID server_entt_id;
    std::string username;
    Team team;
    PlayerSkin skin;
    PlayerState state;
    Vec2D position;
    float angle_direction;  // a dónde está mirando el player
    TypeWeapon weapon_type;
    ServerEntityID equipped_weapon_id;

    PlayerInfo();
    PlayerInfo(ServerEntityID server_entt_id, const std::string& name, Team team, PlayerSkin skin,
               PlayerState state, const Vec2D& position, float angle_direction,
               TypeWeapon weapon_type, ServerEntityID equipped_weapon_id);

    SpriteType generateSpriteType() const;


    PlayerInfo(const PlayerInfo& other) = default;
    PlayerInfo& operator=(const PlayerInfo& other) = default;


    // private:
    explicit PlayerInfo(const std::vector<uint8_t>& bytes);

    /**
     * primeros 2 bytes (en big endian) para el length del nombre.
     * <username>
     * <uint8_t Team>
     * <uint8_t PlayerSkin>
     * <uint8_t PlayerState>
     *
     * <uint16_t pos_x>
     * <uint16_t pos_y>
     * <float direction.x>
     * <float direction.y>
     * <uint8_t weapon_selected>
     * <uint8_t health>
     * <uint16_t money>
     * <uint16_t ammo_weapon>
     */
    std::vector<uint8_t> toBytes() const;

    void print() const;
};

#endif  // PLAYER_INFO_H_
