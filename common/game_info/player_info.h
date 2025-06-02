#ifndef PLAYER_INFO_H_
#define PLAYER_INFO_H_

#include <cstdint>
#include <string>
#include <vector>

#include "../types.h"

struct PlayerInfo {
    unsigned int server_entt_id;
    std::string username;  // id
    Team team;
    PlayerSkin skin;
    PlayerState state;
    int pos_x;
    int pos_y;
    Vec2D direction;  // a dónde está mirando el player (Quizas es mejor que sea un angulo)
    TypeWeapon weapon_selected;
    int health;
    int money;
    int ammo_weapon;

    PlayerInfo();
    PlayerInfo(unsigned int server_entt_id, const std::string& username, Team team, PlayerSkin skin,
               int pos_x, int pos_y, const Vec2D& direction, TypeWeapon weapon, int health,
               int money, int ammo);

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
};

#endif  // PLAYER_INFO_H_
