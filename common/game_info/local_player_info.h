#ifndef HUD_INFO_H_
#define HUD_INFO_H_


#include <cstdint>
#include <vector>

#include "../../client/include/model/utils/SpriteType.h"
#include "../types.h"

#define SIZE_LOCAL_PLAYER_INFO 29

// class GameInfo;  // forward declaration


struct LocalPlayerInfo {
    unsigned int server_entt_id;
    Team team;
    PlayerSkin skin;
    PlayerState state;
    Vec2D position;
    float angle_direction;
    Weapon weapon_selected;
    int health;
    int money;
    int ammo_weapon;
    unsigned int id_weapon;

    LocalPlayerInfo() {}

    explicit LocalPlayerInfo(unsigned int server_entt_id, Team team, PlayerSkin skin,
                             const Vec2D& position, float angle_direction, Weapon weapon,
                             int health, int money, int ammo, unsigned int id_weapon);

    LocalPlayerInfo(const LocalPlayerInfo& other) = default;
    LocalPlayerInfo& operator=(const LocalPlayerInfo& other) = default;

    SpriteType generateSpriteType() const;

    void print() const;

    // private:  // metodos solo utilizados por GameInfo
    //     friend class GameInfo;

    explicit LocalPlayerInfo(const std::vector<uint8_t>& bytes);

    std::vector<uint8_t> toBytes() const;
};

#endif
