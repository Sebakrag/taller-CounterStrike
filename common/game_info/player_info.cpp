#include "player_info.h"

#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>

#include "../protocol.h"
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


std::vector<uint8_t> PlayerInfo::toBytes() const {
    std::vector<uint8_t> buffer;
    int lengthName = username.length();
    Protocol_::insertBigEndian16(lengthName, buffer);
    Protocol_::insertStringBytes(username, buffer);

    buffer.push_back(Protocol_::encodeTeam(team));
    buffer.push_back(Protocol_::encodePlayerSkin(skin));
    buffer.push_back(Protocol_::encodePlayerState(state));
    Protocol_::insertBigEndian16(pos_x, buffer);
    Protocol_::insertBigEndian16(pos_y, buffer);
    Protocol_::insertFloatNormalized3Bytes(direction.x, buffer);
    Protocol_::insertFloatNormalized3Bytes(direction.y, buffer);
    buffer.push_back(Protocol_::encodeTypeWeapon(weapon_selected));
    buffer.push_back(health);
    Protocol_::insertBigEndian16(money, buffer);
    Protocol_::insertBigEndian16(ammo_weapon, buffer);
    return buffer;
}

PlayerInfo::PlayerInfo(const std::vector<uint8_t>& bytes) {
    int index = 0;

    // Leer longitud del nombre y el nombre
    int lengthName = Protocol_::getValueBigEndian16(bytes[0], bytes[1]);
    std::cout << "Length: " << std::to_string(lengthName) << std::endl;
    index += 2;

    username = std::string(bytes.begin() + index, bytes.begin() + index + lengthName);
    index += lengthName;

    // Leer equipo, skin y estado
    team = Protocol_::decodeTeam(bytes[index++]);
    skin = Protocol_::decodePlayerSkin(bytes[index++]);
    state = Protocol_::decodePlayerState(bytes[index++]);

    // Leer posición
    pos_x = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
    index += 2;
    pos_y = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
    index += 2;

    // Leer dirección
    // direction.x = Protocol_::getFloatNormalized(bytes, index);
    direction.x = 0;
    index += 3;
    // direction.y = Protocol_::recvFloatNormalized3Bytes(bytes, index);
    direction.y = 1;
    index += 3;

    // Leer arma, salud, dinero y munición
    weapon_selected = Protocol_::decodeTypeWeapon(bytes[index++]);
    health = bytes[index++];
    money = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
    index += 2;
    ammo_weapon = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
}
