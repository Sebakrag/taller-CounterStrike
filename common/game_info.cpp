#include "game_info.h"

#include <cstdint>
#include <vector>

#include "constants_protocol.h"

GameInfo::GameInfo(GamePhase gamePhase, const std::vector<PlayerInfo>& players):
        gamePhase(gamePhase), players(players) {}

GameInfo::GameInfo(const GameInfo& other): gamePhase(other.gamePhase), players(other.players) {}

GameInfo& GameInfo::operator=(const GameInfo& other) {
    gamePhase = other.gamePhase;
    players = other.players;
    return *this;
}

std::vector<uint8_t> GameInfo::toBytes() const {
    std::vector<uint8_t> buffer;
    buffer.push_back(BYTE_GAME_INFO);

    // YOUR CODE

    return buffer;
}
