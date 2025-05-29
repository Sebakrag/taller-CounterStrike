#include "player_info.h"

PlayerInfo::PlayerInfo():
        team(Team::Terrorist),
        skin(PlayerSkin()),
        state(PlayerState::Idle),
        pos_x(0),
        pos_y(0),
        direction(0, 1),
        weapon_selected(TypeWeapon::Knife),
        health(100),
        money(100),
        ammo_weapon(10) {}

PlayerInfo::PlayerInfo(const std::string& username, Team team, PlayerSkin skin, int pos_x,
                       int pos_y, const Vector2& direction, TypeWeapon weapon, int health,
                       int money, int ammo):
        username(username),
        team(team),
        skin(skin),
        state(PlayerState::Idle),
        pos_x(pos_x),
        pos_y(pos_y),
        direction(direction),
        weapon_selected(weapon),
        health(health),
        money(money),
        ammo_weapon(ammo) {}
