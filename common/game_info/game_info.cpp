#include "game_info.h"

#include <cstdint>
#include <iostream>
#include <vector>

#include "../protocol.h"

#include "bullet_info.h"
#include "local_player_info.h"
#include "player_info.h"

// GameInfo::GameInfo():
//         gamePhase(GamePhase::Preparation), bombState(false), bombX(0), bombY(0), timeLeft(30)
//         {}

GameInfo::GameInfo(const GamePhase gamePhase, const BombInfo& bomb, const float timeLeft,
                   const LocalPlayerInfo& localPlayer, const std::vector<PlayerInfo>& otherPlayers,
                   const std::vector<BulletInfo>& bullets, const std::vector<WeaponInfo>& items,
                   const StatsInfo& stats):
        gamePhase(gamePhase),
        bomb(bomb),
        timeLeft(timeLeft),
        localPlayer(localPlayer),
        otherPlayers(otherPlayers),
        bullets(bullets),
        weapons(items),
        stats(stats) {}

// TODO: modularizar. (Hacer 4 metodos privados)
GameInfo::GameInfo(const std::vector<uint8_t>& bytes) {
    // GamePhase (1 byte)
    gamePhase = Protocol_::decodeGamePhase(bytes[0]);
    size_t index = 1;

    // Bomb
    std::vector<uint8_t> bombBytes(bytes.begin() + index, bytes.begin() + index + SIZE_BOMB_INFO);
    bomb = BombInfo(bombBytes);
    index += SIZE_BOMB_INFO;

    EntitySnapshot bombEntt(bomb.server_entt_id, EntityType::BOMB, bomb.getSpriteType(), bomb.pos_x,
                            bomb.pos_y, 0, true, bomb.state);
    entities.emplace_back(bombEntt);

    // Time left
    timeLeft =
            Protocol_::getFloat(bytes[index], bytes[index + 1], bytes[index + 2], bytes[index + 3]);
    index += 4;

    // local Player.
    std::vector<uint8_t> localPlayerBytes(bytes.begin() + index,
                                          bytes.begin() + index + SIZE_LOCAL_PLAYER_INFO);
    localPlayer = LocalPlayerInfo(localPlayerBytes);
    index += SIZE_LOCAL_PLAYER_INFO;
    // cargo snapshot
    const auto x = localPlayer.position.getX();
    const auto y = localPlayer.position.getY();
    EntitySnapshot entity(localPlayer.server_entt_id, EntityType::PLAYER,
                          localPlayer.generateSpriteType(), x, y, localPlayer.angle_direction, true,
                          localPlayer.health, localPlayer.money, localPlayer.state,
                          localPlayer.equipped_weapon_id, localPlayer.weapon, localPlayer.team,
                          localPlayer.weapon_type);
    entities.emplace_back(entity);

    // other players
    uint16_t numPlayers = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
    index += 2;

    for (uint16_t i = 0; i < numPlayers; i++) {
        uint16_t size = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
        index += 2;

        std::vector<uint8_t> playerBytes(bytes.begin() + index, bytes.begin() + index + size);

        PlayerInfo p(playerBytes);
        otherPlayers.emplace_back(p);
        const float x = p.position.getX();
        const float y = p.position.getY();

        EntitySnapshot entity(p.server_entt_id, EntityType::PLAYER, p.generateSpriteType(), x, y,
                              p.angle_direction, true, p.state, p.equipped_weapon_id, p.weapon,
                              p.team, p.weapon_type);
        entities.emplace_back(entity);

        index += size;
    }

    // Bullets
    uint16_t numBullets = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
    index += 2;

    for (uint16_t i = 0; i < numBullets; i++) {
        std::vector<uint8_t> bulletBytes(bytes.begin() + index,
                                         bytes.begin() + index + SIZE_BULLET_INFO);

        BulletInfo b(bulletBytes);
        bullets.emplace_back(b);
        EntitySnapshot entity(b.id, EntityType::BULLET, SpriteType::BULLET, b.pos_x, b.pos_y,
                              b.direction.calculateAngleDegrees(), b.active);
        entities.emplace_back(entity);

        index += SIZE_BULLET_INFO;
    }

    // Weapons
    uint16_t numWeapons = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
    index += 2;

    for (uint16_t i = 0; i < numWeapons; i++) {
        std::vector<uint8_t> weaponBytes(bytes.begin() + index,
                                         bytes.begin() + index + SIZE_ITEM_INFO);

        WeaponInfo weapon(weaponBytes);
        weapons.emplace_back(weapon);
        EntitySnapshot entity(weapon.server_entt_id, EntityType::WEAPON, weapon.getSpriteType(),
                              weapon.pos_x, weapon.pos_y, 0, true, weapon.state);
        entities.emplace_back(entity);
        index += SIZE_ITEM_INFO;
    }

    // stats
    if (gamePhase == GamePhase::EndOfMatch) {
        uint16_t statsSize = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
        index += 2;
        std::vector<uint8_t> statsBytes(bytes.begin() + index, bytes.begin() + index + statsSize);
        stats = StatsInfo(statsBytes);
    }
}

std::vector<uint8_t> GameInfo::toBytes() const {
    std::vector<uint8_t> buffer;

    buffer.push_back(Protocol_::encodeGamePhase(gamePhase));

    // bomb
    for (uint8_t& b: bomb.toBytes()) {
        buffer.push_back(b);
    }

    // time
    Protocol_::insertFloat4Bytes(timeLeft, buffer);

    // localPlayer
    for (uint8_t& b: localPlayer.toBytes()) {
        buffer.push_back(b);
    }

    // otherPlayers.
    Protocol_::insertBigEndian16(otherPlayers.size(), buffer);
    for (auto& p: otherPlayers) {
        auto p_bytes = p.toBytes();
        Protocol_::insertBigEndian16(p_bytes.size(), buffer);
        for (uint8_t byte: p_bytes) {
            buffer.push_back(byte);
        }
    }

    // bullets.
    Protocol_::insertBigEndian16(bullets.size(), buffer);
    for (auto& b: bullets) {
        auto b_bytes = b.toBytes();
        for (uint8_t byte: b_bytes) {
            buffer.push_back(byte);
        }
    }
    // items.
    Protocol_::insertBigEndian16(weapons.size(), buffer);
    for (const auto& item: weapons) {
        auto item_bytes = item.toBytes();
        buffer.insert(buffer.end(), item_bytes.begin(), item_bytes.end());
    }

    if (gamePhase == GamePhase::EndOfMatch) {
        std::vector<uint8_t> statsBytes = stats.toBytes();
        Protocol_::insertBigEndian16(statsBytes.size(), buffer);
        buffer.insert(buffer.end(), statsBytes.begin(), statsBytes.end());
    }

    return buffer;
}

std::vector<EntitySnapshot> GameInfo::getSnapshots() const { return entities; }

void GameInfo::print() const {
    std::cout << "Game Phase: " << static_cast<int>(gamePhase) << std::endl;

    std::cout << "\nPlayers (" << otherPlayers.size() << "):" << std::endl;
    for (auto& p: otherPlayers) {
        p.print();
    }

    std::cout << "\nBullets (" << bullets.size() << "):" << std::endl;


    std::cout << "\nItems (" << weapons.size() << "):" << std::endl;
    for (const WeaponInfo& weapon: weapons) {
        std::cout << "Type: " << static_cast<int>(weapon.weapon) << ", Ammo: " << weapon.ammo
                  << ", Position: (" << weapon.pos_x << "," << weapon.pos_y << ")" << std::endl;
    }

    std::cout << "\nBomb:" << std::endl;
    std::cout << "Bomb State: " << static_cast<int>(bomb.state) << std::endl;
    std::cout << "Position: (" << bomb.pos_x << "," << bomb.pos_y << ")" << std::endl;

    std::cout << "\nTime Left: " << timeLeft << std::endl;
}
