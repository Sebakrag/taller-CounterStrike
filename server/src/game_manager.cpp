
#include "../include/game_manager.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

/*GameManager::GameManager(const std::list<std::string>& escenarios) {
    // YOUR CODE
}
*/
bool GameManager::createMatch(const std::string& matchName, const std::string& username) {
    // chequeo que no sea un nombre sin caracteres
    if (matchName.empty() || std::all_of(matchName.begin(), matchName.end(), ::isspace)) {
        return false;
    }
    std::lock_guard<std::mutex> lock(m);

    if (server_closed || lobbies.find(matchName) != lobbies.end()) {  // si la partida ya existe
        return false;
    }
    lobbies.try_emplace(matchName, matchName, username);

    std::cout << username << " creó la partida " << matchName << std::endl;

    return true;
}

bool GameManager::JoinMatch(const std::string& matchName, const std::string& username) {
    std::lock_guard<std::mutex> lock(m);

    if (server_closed)
        return false;
    auto it = lobbies.find(matchName);
    if (it == lobbies.end() ||
        it->second.isAvailable() == false) {  // no existe la partida o ya inició
        return false;
    }

    it->second.addPlayer(username);
    std::cout << username << " se unió a partida " << matchName << std::endl;
    return true;
}

bool GameManager::QuitMatch(const std::string& matchName, const std::string& username) {
    std::lock_guard<std::mutex> lock(m);
    auto it = lobbies.find(matchName);
    if (it == lobbies.end() ||
        it->second.isAvailable() == false) {  // no existe la partida o ya inició
        return false;
    }
    it->second.removePlayer(username);
    if (it->second.isEmpty()) {
        lobbies.erase(it);  // elimina la partida vacía
    }
    return true;
}

std::list<std::string> GameManager::listMatchs() {
    std::lock_guard<std::mutex> lock(m);
    std::list<std::string> sorted_matches;
    for (const auto& match: lobbies) {
        sorted_matches.push_back(match.first);
    }
    sorted_matches.sort();  // ordeno alfabeticamente.
    return sorted_matches;
}

bool GameManager::startMatch(const std::string& username, const std::string& matchName) {
    std::lock_guard<std::mutex> lock(m);
    // busco la partida...
    auto it = lobbies.find(matchName);
    if (it == lobbies.end() ||
        it->second.isAvailable() == false) {  // no existe la partida o ya inició
        return false;
    }
    if (it->second.containsPlayer(username) && it->second.isPlayerHost(username)) {
        std::cout << "Acá comenzaría la partida. Todavía no disponible" << std::endl;
        return false;
    }
    std::cout << "No comenzó la partida porque no es el anfitrión." << std::endl;
    return false;
}

std::vector<PlayerInfoLobby> GameManager::getPlayersInMatch(const std::string& matchName) {
    auto it = lobbies.find(matchName);
    if (it == lobbies.end()) {  // no existe la partida
        throw std::runtime_error("No existe la partida.");
    }
    return it->second.getPlayersInRoom();
}

/*
MatchRoom& GameManager::getMatchRoom(const std::string& matchName, const std::string& username) {
    auto it = lobbies.find(matchName);
    if (it == lobbies.end() ||
        it->second.isAvailable() == false) {  // no existe la partida o ya inició
        throw std::runtime_error("Partida no existe o ya comenzó");
    }

    return it->second;
}
*/
