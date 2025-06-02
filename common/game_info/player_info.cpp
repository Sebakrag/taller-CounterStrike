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

PlayerInfo::PlayerInfo(unsigned int server_entt_id, const std::string& username, Team team,
                       PlayerSkin skin, int pos_x, int pos_y, const Vec2D& direction,
                       TypeWeapon weapon, int health, int money, int ammo):
        server_entt_id(server_entt_id),
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

    // EntityId (4 Bytes)
    Protocol_::insertBigEndian32(server_entt_id, buffer);
    // length y nombre de usuario (2 + <Length> bytes)
    const int lengthName = static_cast<int>(username.length());
    Protocol_::insertBigEndian16(lengthName, buffer);
    Protocol_::insertStringBytes(username, buffer);

    // team, skin y state (3 bytes)
    buffer.push_back(Protocol_::encodeTeam(team));
    buffer.push_back(Protocol_::encodePlayerSkin(skin));
    buffer.push_back(Protocol_::encodePlayerState(state));

    // posicion (4 bytes)
    Protocol_::insertBigEndian16(pos_x, buffer);
    Protocol_::insertBigEndian16(pos_y, buffer);

    Protocol_::insertFloatNormalized3Bytes(direction.getX(), buffer);
    Protocol_::insertFloatNormalized3Bytes(direction.getY(), buffer);

    buffer.push_back(Protocol_::encodeTypeWeapon(weapon_selected));
    buffer.push_back(health);
    Protocol_::insertBigEndian16(money, buffer);
    Protocol_::insertBigEndian16(ammo_weapon, buffer);
    return buffer;
}

PlayerInfo::PlayerInfo(const std::vector<uint8_t>& bytes) {
    int index = 0;
    server_entt_id = Protocol_::getBigEndian32(bytes[0], bytes[1], bytes[2], bytes[3]);
    index += 4;
    // Leer longitud del nombre y el nombre
    int lengthName = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
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
    // direction.setX(Protocol_::getFloatNormalized(bytes, index));
    direction.setX(0);
    index += 3;
    // direction.setY(Protocol_::recvFloatNormalized3Bytes(bytes, index));
    direction.setY(1);
    index += 3;

    // Leer arma, salud, dinero y munición
    weapon_selected = Protocol_::decodeTypeWeapon(bytes[index++]);
    health = bytes[index++];
    money = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
    index += 2;
    ammo_weapon = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
}
