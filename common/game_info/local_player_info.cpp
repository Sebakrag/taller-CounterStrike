#include "local_player_info.h"

#include "../protocol.h"

LocalPlayerInfo::LocalPlayerInfo(const ServerEntityID server_entt_id, const Team team,
                                 const PlayerSkin skin, const Vec2D& position,
                                 const float angle_direction, const TypeWeapon weapon_type,
                                 const int health, const int money, const int ammo,
                                 const ServerEntityID equipped_weapon_id):
        server_entt_id(server_entt_id),
        team(team),
        skin(skin),
        state(PlayerState::Idle),
        position(position.getX(), position.getY()),
        angle_direction(angle_direction),
        weapon_type(weapon_type),
        health(health),
        money(money),
        ammo_weapon(ammo),
        equipped_weapon_id(equipped_weapon_id) {}

SpriteType LocalPlayerInfo::generateSpriteType() const {
    switch (skin) {
        case PlayerSkin::Terrorist1:
            return SpriteType::PHEONIX;
        case PlayerSkin::Terrorist2:
            return SpriteType::L337_KREW;
        case PlayerSkin::Terrorist3:
            return SpriteType::ARTIC_AVENGER;
        case PlayerSkin::Terrorist4:
            return SpriteType::GUERRILLA;
        case PlayerSkin::CounterTerrorist1:
            return SpriteType::SEAL_FORCE;
        case PlayerSkin::CounterTerrorist2:
            return SpriteType::GERMAN_GSG_9;
        case PlayerSkin::CounterTerrorist3:
            return SpriteType::UK_SAS;
        case PlayerSkin::CounterTerrorist4:
            return SpriteType::FRENCH_GIGN;
        default:
            if (team == Team::Terrorist)
                return SpriteType::ARTIC_AVENGER;
            else
                return SpriteType::FRENCH_GIGN;
    }
}

std::vector<uint8_t> LocalPlayerInfo::toBytes() const {
    std::vector<uint8_t> buffer;
    // EntityId (4 Bytes)
    Protocol_::insertBigEndian32(server_entt_id, buffer);

    // team, skin y state (3 bytes)
    buffer.push_back(Protocol_::encodeTeam(team));
    buffer.push_back(Protocol_::encodePlayerSkin(skin));
    buffer.push_back(Protocol_::encodePlayerState(state));

    // posicion (8 bytes)
    Protocol_::insertFloat4Bytes(position.getX(), buffer);
    Protocol_::insertFloat4Bytes(position.getY(), buffer);

    // angulo de direccion (4 bytes)
    Protocol_::insertFloat4Bytes(angle_direction, buffer);

    // 6 bytes
    buffer.push_back(Protocol_::encodeTypeWeapon(weapon_type));
    buffer.push_back(health);
    Protocol_::insertBigEndian16(money, buffer);
    Protocol_::insertBigEndian16(ammo_weapon, buffer);

    Protocol_::insertBigEndian32(equipped_weapon_id, buffer);

    return buffer;
}


LocalPlayerInfo::LocalPlayerInfo(const std::vector<uint8_t>& bytes) {
    int index = 0;
    server_entt_id = Protocol_::getBigEndian32(bytes[0], bytes[1], bytes[2], bytes[3]);
    index += 4;

    // Leer equipo, skin y estado
    team = Protocol_::decodeTeam(bytes[index++]);
    skin = Protocol_::decodePlayerSkin(bytes[index++]);
    state = Protocol_::decodePlayerState(bytes[index++]);

    // Leer posición
    float pos_x =
            Protocol_::getFloat(bytes[index], bytes[index + 1], bytes[index + 2], bytes[index + 3]);
    index += 4;
    float pos_y =
            Protocol_::getFloat(bytes[index], bytes[index + 1], bytes[index + 2], bytes[index + 3]);
    index += 4;
    position.set(pos_x, pos_y);

    // Leer angulo
    angle_direction =
            Protocol_::getFloat(bytes[index], bytes[index + 1], bytes[index + 2], bytes[index + 3]);
    index += 4;

    // Leer arma, salud, dinero y munición
    weapon_type = Protocol_::decodeTypeWeapon(bytes[index++]);
    health = bytes[index++];
    money = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
    index += 2;
    ammo_weapon = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
    index += 2;

    equipped_weapon_id = Protocol_::getBigEndian32(bytes[index], bytes[index + 1], bytes[index + 2],
                                                   bytes[index + 3]);
    index += 4;
}

void LocalPlayerInfo::print() const {
    std::cout << "===== Player Info =====" << std::endl;
    std::cout << "Server Entity ID: " << server_entt_id << std::endl;
    std::cout << "Team: " << static_cast<int>(team) << std::endl;
    std::cout << "Skin: " << static_cast<int>(skin) << std::endl;
    std::cout << "State: " << static_cast<int>(state) << std::endl;
    std::cout << "Position: (" << position.getX() << ", " << position.getY() << ")" << std::endl;
    std::cout << "Angle Direction: " << angle_direction << " radians" << std::endl;
    std::cout << "Weapon Selected: " << static_cast<int>(weapon_type) << std::endl;
    std::cout << "Weapon id: " << static_cast<int>(equipped_weapon_id) << std::endl;
    std::cout << "Health: " << health << std::endl;
    std::cout << "Money: $" << money << std::endl;
    std::cout << "Ammo: " << ammo_weapon << std::endl;
    std::cout << "========================" << std::endl;
}
