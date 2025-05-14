#include "game_info.h"

GameInfo::GameInfo(GamePhase gamePhase, const std::vector<PlayerInfo>& players):
        gamePhase(gamePhase), players(players) {}
