#include "match.h"

#include <algorithm>

Match::Match(const std::string& username): playerHost(username) {}

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

bool Match::isAvailable() {
    // TODO: implementar lógica para verificar si hay espacio
    return true;
}

std::vector<std::string> Match::getPlayers() { return players; }

Match::~Match() {
    // TODO: implementar limpieza de recursos
}

void Match::run() {
    // TODO: implementar lógica principal del juego
    while (should_keep_running()) {
        // Lógica del juego
    }
}

void Match::start() { Thread::start(); }
