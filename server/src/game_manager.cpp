
#include "../include/game_manager.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

#include "../include/scenario_registry.h"

/*GameManager::GameManager(const std::list<std::string>& escenarios) {
    // YOUR CODE
}*/
bool GameManager::createMatch(const std::string& matchName, const std::string& username,
                              std::shared_ptr<Queue<GameInfo>> playerQueue,
                              const std::string& id_scenary) {
    // chequeo que no sea un nombre sin caracteres
    if (matchName.empty() || std::all_of(matchName.begin(), matchName.end(), ::isspace)) {
        return false;
    }
    if (!ScenarioRegistry::existsScenario(id_scenary)) {
        return false;
    }
    std::lock_guard<std::mutex> lock(m);

    if (server_closed || lobbies.find(matchName) != lobbies.end()) {  // si la partida ya existe
        return false;
    }
    lobbies.try_emplace(matchName, matchName, username, playerQueue, id_scenary);

    return true;
}

bool GameManager::JoinMatch(const std::string& matchName, const std::string& username,
                            std::shared_ptr<Queue<GameInfo>> playerQueue) {
    std::lock_guard<std::mutex> lock(m);

    if (server_closed)
        return false;
    auto it = lobbies.find(matchName);
    if (it == lobbies.end() ||
        it->second.isAvailable() == false) {  // no existe la partida o ya inició
        return false;
    }

    it->second.addPlayer(username, playerQueue);
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

bool GameManager::StartMatch(const std::string& username, const std::string& matchName) {
    std::lock_guard<std::mutex> lock(m);
    // busco la partida...
    auto it = lobbies.find(matchName);
    if (it == lobbies.end() ||
        it->second.isAvailable() == false) {  // no existe la partida o ya inició
        return false;
    }
    if (it->second.containsPlayer(username) && it->second.isPlayerHost(username)) {
        auto gameloop = it->second.createGameLoop();
        gameloop->start();
        gameLoops.try_emplace(matchName, std::move(gameloop));
        // lobbies.erase(matchName);
        return true;
    }

    return false;
}

MatchRoomInfo GameManager::getMatchRoomInfo(const std::string& matchName) {
    auto it = lobbies.find(matchName);
    if (it == lobbies.end()) {  // no existe la partida
        throw std::runtime_error("No existe la partida.");
    }
    return it->second.getMatchRoomInfo();
}

MatchInfo GameManager::getMatchInfo(const std::string& matchName, const std::string& username) {
    auto it2 = gameLoops.find(matchName);
    if (it2 == gameLoops.end()) {  // no existe la partida
        throw std::runtime_error("No existe la partida.");
    }
    const Match& match = it2->second->getMatch();
    if (match.containsPlayer(username) == false) {
        throw std::runtime_error("El usuario no está unido a esa partida.");
    }
    auto localPlayerInfo = match.generateLocalPlayerInfo(username);
    std::string id_scenary = match.getIdScenario();
    int numPlayers = match.countPlayers();

    return MatchInfo(matchName, ScenarioRegistry::getWindowConfig(),
                     ScenarioRegistry::getFovConfig(), ScenarioRegistry::getTileMap(id_scenary),
                     numPlayers, localPlayerInfo, Shop::getInfo());
}

std::shared_ptr<Queue<PlayerAction>> GameManager::getActionsQueue(const std::string& matchName) {
    std::lock_guard<std::mutex> lock(m);
    return gameLoops.at(matchName)->getActionsQueue();
}

void GameManager::reapDeadGameloops() {
    for (auto it = gameLoops.begin(); it != gameLoops.end();) {
        if (!it->second->is_alive()) {
            // borro matchRoom
            std::string nameMatch = it->first;
            deleteMatchRoom(nameMatch);
            // borro gameloop
            it->second->join();
            it = gameLoops.erase(it);  // borra y avanza el iterador
        } else {
            ++it;
        }
    }
}

void GameManager::deleteMatchRoom(const std::string& matchName) {
    auto it = lobbies.find(matchName);
    if (it != lobbies.end()) {
        lobbies.erase(it);
    }
}


// asegurarse de que solo la clase server pueda llamar a este metodo.
void GameManager::killAllMatchs() {
    std::lock_guard<std::mutex> lock(m);
    reapDeadGameloops();
    for (auto& [matchName, gameLoop]: gameLoops) {
        gameLoop->kill();
        gameLoop->join();
    }
    gameLoops.clear();
}
