#include "game_info.h"

#include <cstdint>
#include <vector>

#include "../constants_protocol.h"
#include "../protocol.h"

GameInfo::GameInfo():
        gamePhase(GamePhase::Preparation), bombPlanted(false), bombX(0), bombY(0), timeLeft(30) {}

GameInfo::GameInfo(GamePhase gamePhase, double timeLeft, const std::vector<PlayerInfo>& players):
        gamePhase(gamePhase),
        bombPlanted(false),
        bombX(0),
        bombY(0),
        timeLeft(timeLeft),
        players(players) {}

GameInfo::GameInfo(GamePhase gamePhase, bool bombPlanted, int bombX, int bombY, double timeLeft,
                   const std::vector<PlayerInfo>& players, const std::vector<BulletInfo>& bullets,
                   const std::vector<ItemsInfo>& items):
        gamePhase(gamePhase),
        bombPlanted(bombPlanted),
        bombX(bombX),
        bombY(bombY),
        timeLeft(timeLeft),
        players(players),
        bullets(bullets),
        items(items) {}

GameInfo::GameInfo(const GameInfo& other):
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


std::vector<uint8_t> GameInfo::toBytes() const {
    std::vector<uint8_t> buffer;
    buffer.push_back(Protocol_::encodeGamePhase(gamePhase));

    // YOUR CODE

    return buffer;
}
