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

GameInfo::GameInfo(GamePhase gamePhase, float timeLeft, const std::vector<PlayerInfo>& players):
        gamePhase(gamePhase),
        bombPlanted(false),
        bombX(0),
        bombY(0),
        timeLeft(timeLeft),
        players(players) {}

GameInfo::GameInfo(GamePhase gamePhase, bool bombPlanted, int bombX, int bombY, float timeLeft,
                   const std::vector<PlayerInfo>& players, const std::vector<BulletInfo>& bullets,
                   const std::vector<ItemInfo>& items):
        gamePhase(gamePhase),
        bombPlanted(bombPlanted),
        bombX(bombX),
        bombY(bombY),
        timeLeft(timeLeft),
        players(players),
        bullets(bullets),
        items(items) {}

GameInfo::GameInfo(const GameInfo& other):
        entities(other.entities),
        gamePhase(other.gamePhase),
        bombPlanted(other.bombPlanted),
        bombX(other.bombX),
        bombY(other.bombY),
        timeLeft(other.timeLeft),
        players(other.players),
        bullets(other.bullets),
        items(other.items) {}

GameInfo& GameInfo::operator=(const GameInfo& other) {
    if (this != &other) {
        entities = other.entities;
        gamePhase = other.gamePhase;
        bombPlanted = other.bombPlanted;
        bombX = other.bombX;
        bombY = other.bombY;
        timeLeft = other.timeLeft;
        players = other.players;
        bullets = other.bullets;
        items = other.items;
    }
    return *this;
}

// TODO: modularizar. (Hacer 4 metodos privados)
GameInfo::GameInfo(const std::vector<uint8_t>& bytes) {
    // GamePhase (1 byte)
    gamePhase = Protocol_::decodeGamePhase(bytes[0]);
    size_t index = 1;

    // Players
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

        EntitySnapshot entity(p.server_entt_id, p.position.getX(), p.position.getY(),
                              p.angle_direction, SpriteType::ARTIC_AVENGER, EntityType::TERRORIST,
                              p.health, p.money, p.team, p.state);
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

        EntitySnapshot entity(b.id, b.pos_x, b.pos_y, b.direction.calculateAngle(),
                              SpriteType::BULLET, EntityType::BULLET);
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

        EntitySnapshot entity(item.server_entt_id, item.pos_x, item.pos_y, item.getSpriteType(),
                              EntityType::DROP);
        entities.emplace_back(entity);
        index += SIZE_ITEM_INFO;
    }

    // Bomb info
    bombPlanted = Protocol_::decodeBool(bytes[index]);
    index += 1;
    bombX = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
    index += 2;
    bombY = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
    index += 2;

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
    // bomb
    buffer.push_back(Protocol_::encodeBool(bombPlanted));
    Protocol_::insertBigEndian16(bombX, buffer);
    Protocol_::insertBigEndian16(bombY, buffer);

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
    std::cout << "Planted: " << (bombPlanted ? "Yes" : "No") << std::endl;
    std::cout << "Position: (" << bombX << "," << bombY << ")" << std::endl;

    std::cout << "\nTime Left: " << timeLeft << std::endl;
}
