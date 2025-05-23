#include "../include/match.h"

#include <algorithm>
#include <iostream>
#include <vector>

Match::Match() {}

bool Match::addPlayer(const std::string& username) {
    players.push_back(username);
    return true;
}

void Match::removePlayer(const std::string& username) {
    auto it = std::find(players.begin(), players.end(), username);
    if (it != players.end()) {
        players.erase(it);
    }
}


std::vector<std::string> Match::getPlayers() { return players; }

void Match::processAction(const PlayerAction& action) {
    std::cout << "Acá ejecuto la accion de " << action.player_username << std::endl;
}

GameInfo Match::generateGameInfo() const {
    std::vector<PlayerInfo> playersInfo;
    return GameInfo(GamePhase::Preparation, playersInfo);
}
