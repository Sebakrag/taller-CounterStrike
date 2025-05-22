#include "game_info.h"

GameInfo::GameInfo(GamePhase gamePhase, const std::vector<PlayerInfo>& players):
        gamePhase(gamePhase), players(players) {}

GameInfo::GameInfo(const GameInfo& other): gamePhase(other.gamePhase), players(other.players) {}

GameInfo& GameInfo::operator=(const GameInfo& other) {
    gamePhase = other.gamePhase;
    players = other.players;
    return *this;
}
