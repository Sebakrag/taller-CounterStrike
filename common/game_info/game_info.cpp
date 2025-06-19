#include "game_info.h"

#include <cstdint>
#include <iostream>
#include <vector>

#include "../protocol.h"

#include "bullet_info.h"
#include "player_info.h"

// GameInfo::GameInfo():
//         gamePhase(GamePhase::Preparation), bombPlanted(false), bombX(0), bombY(0), timeLeft(30)
//         {}

GameInfo::GameInfo(GamePhase gamePhase, float timeLeft, const BombInfo& bombInfo, const ShopInfo& shopInfo,
                   const std::vector<PlayerInfo>& players, const std::vector<ProjectileInfo>& projectiles):
        gamePhase(gamePhase),
        bombInfo(bombInfo),
        shopInfo(shopInfo),
        timeLeft(timeLeft),
        players(players),
        projectiles(projectiles) {}

GameInfo::GameInfo(GamePhase gamePhase, const BombInfo& bombInfo, const ShopInfo& shopInfo, float timeLeft,
                   const std::vector<PlayerInfo>& players, const std::vector<BulletInfo>& bullets,
                   const std::vector<ItemInfo>& items):
        gamePhase(gamePhase),
        bombInfo(bombInfo),
        shopInfo(shopInfo),
        timeLeft(timeLeft),
        players(players),
        bullets(bullets),
        items(items) {}

GameInfo::GameInfo(const GameInfo& other):
        entities(other.entities),
        gamePhase(other.gamePhase),
        bombInfo(other.bombInfo),
        shopInfo(other.shopInfo),
        timeLeft(other.timeLeft),
        players(other.players),
        bullets(other.bullets),
        items(other.items),
        projectiles(other.projectiles) {}

GameInfo& GameInfo::operator=(const GameInfo& other) {
    if (this != &other) {
        entities = other.entities;
        gamePhase = other.gamePhase;
        bombInfo = other.bombInfo;
        shopInfo = other.shopInfo;
        timeLeft = other.timeLeft;
        players = other.players;
        bullets = other.bullets;
        items = other.items;
        projectiles = other.projectiles;
    }
    return *this;
}

// TODO: modularizar. (Hacer 4 metodos privados)
GameInfo::GameInfo(const std::vector<uint8_t>& bytes) {
    // GamePhase (1 byte)
    gamePhase = Protocol_::decodeGamePhase(bytes[0]);

    // Players
    size_t index = 1;
    uint16_t numPlayers = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
    index += 2;

    for (uint16_t i = 0; i < numPlayers; i++) {
        uint16_t size = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
        index += 2;

        std::vector<uint8_t> playerBytes(
                bytes.begin() + index,
                bytes.begin() + index + size);  // 19 bytes después del username

        players.emplace_back(playerBytes);
        PlayerInfo& p = players.back();

        const auto x = static_cast<float>(p.position.getX());
        const auto y = static_cast<float>(p.position.getY());

        EntitySnapshot entity(p.server_entt_id, EntityType::PLAYER, SpriteType::ARTIC_AVENGER, x, y,
                              p.angle_direction, true, p.health, p.money, 0, p.state, 6, p.team);
        entities.emplace_back(entity);

        index += size;
    }

    // Bullets
    uint16_t numBullets = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
    index += 2;

    for (uint16_t i = 0; i < numBullets; i++) {
        std::vector<uint8_t> bulletBytes(bytes.begin() + index,
                                         bytes.begin() + index + SIZE_BULLET_INFO);
        bullets.emplace_back(bulletBytes);
        BulletInfo& b = bullets.back();

        const auto x = static_cast<float>(b.pos_x);
        const auto y = static_cast<float>(b.pos_y);

        EntitySnapshot entity(b.id, EntityType::BULLET, SpriteType::BULLET, x, y,
                              b.direction.calculateAngle(), true);

        entities.emplace_back(entity);
        index += SIZE_BULLET_INFO;
    }

    // Items
    uint16_t numItems = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
    index += 2;

    for (uint16_t i = 0; i < numItems; i++) {
        std::vector<uint8_t> itemBytes(bytes.begin() + index,
                                       bytes.begin() + index + SIZE_ITEM_INFO);
        items.emplace_back(itemBytes);
        ItemInfo& item = items.back();

        const auto x = static_cast<float>(item.pos_x);
        const auto y = static_cast<float>(item.pos_y);
        // TODO: esta bueno generalizar a items dropeados. Pero como por ahora solo tenemos
        // armas dropeadas lo simplificamos.
        EntitySnapshot entity(item.server_entt_id, EntityType::WEAPON, item.getSpriteType(), x, y,
                              0, true, WeaponState::DROPPED);
        entities.emplace_back(entity);
        index += SIZE_ITEM_INFO;
    }

    // Projectiles info
    uint16_t numProjectiles = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
    index += 2;

    for (uint16_t i = 0; i < numProjectiles; ++i) {
        size_t projStart = index;

        size_t lenOffset = projStart + 16;
        uint16_t nameLen = Protocol_::getValueBigEndian16(bytes[lenOffset], bytes[lenOffset + 1]);
        size_t totalSize = 18 + nameLen;

        std::vector<uint8_t> projBytes(bytes.begin() + index, bytes.begin() + index + totalSize);
        projectiles.emplace_back(projBytes);
        index += totalSize;
    }

    // Bomb info
    std::vector<uint8_t> bombBytes(bytes.begin() + index, bytes.begin() + index + 21);
    bombInfo = BombInfo(bombBytes);
    index += 21;

    // Shop info
    size_t remaining = bytes.size() - index - 4;
    std::vector<uint8_t> shopBytes(bytes.begin() + index, bytes.begin() + index + remaining);
    shopInfo = ShopInfo(shopBytes);
    index += remaining;

    // Time left
    timeLeft =
            Protocol_::getFloat(bytes[index], bytes[index + 1], bytes[index + 2], bytes[index + 3]);
}

std::vector<uint8_t> GameInfo::toBytes() const {
    std::vector<uint8_t> buffer;

    buffer.push_back(Protocol_::encodeGamePhase(gamePhase));

    // players.
    Protocol_::insertBigEndian16(players.size(), buffer);
    for (auto& p: players) {
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
    Protocol_::insertBigEndian16(items.size(), buffer);
    for (const auto& item: items) {
        auto item_bytes = item.toBytes();
        buffer.insert(buffer.end(), item_bytes.begin(), item_bytes.end());
    }

    // Projectiles
    Protocol_::insertBigEndian16(projectiles.size(), buffer);
    for (const auto& proj: projectiles) {
        auto bytes = proj.toBytes();
        buffer.insert(buffer.end(), bytes.begin(), bytes.end());
    }

    // bomb
    auto bombBytes = bombInfo.toBytes();
    buffer.insert(buffer.end(), bombBytes.begin(), bombBytes.end());

    // shop
    auto shopBytes = shopInfo.toBytes();
    buffer.insert(buffer.end(), shopBytes.begin(), shopBytes.end());

    // time
    Protocol_::insertFloat4Bytes(timeLeft, buffer);

    return buffer;
}

std::vector<EntitySnapshot> GameInfo::getSnapshots() { return entities; }

void GameInfo::print() const {
    std::cout << "Game Phase: " << static_cast<int>(gamePhase) << std::endl;

    std::cout << "\nPlayers (" << players.size() << "):" << std::endl;
    for (auto& p: players) {
        p.print();
    }

    std::cout << "\nBullets (" << bullets.size() << "):" << std::endl;


    std::cout << "\nItems (" << items.size() << "):" << std::endl;
    for (const ItemInfo& item: items) {
        std::cout << "Type: " << static_cast<int>(item.type) << ", Ammo: " << item.ammo
                  << ", Position: (" << item.pos_x << "," << item.pos_y << ")" << std::endl;
    }

    std::cout << "\nBomb:" << std::endl;
    bombInfo.print();

    std::cout << "\nTime Left: " << timeLeft << std::endl;
}
