
#include "../include/game_manager.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

#include "../include/scenario_registry.h"

/*GameManager::GameManager(const std::list<std::string>& escenarios) {
    // YOUR CODE
}
*/
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

    std::cout << username << " creó la partida " << matchName << std::endl;

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

bool GameManager::StartMatch(const std::string& username, const std::string& matchName) {
    std::lock_guard<std::mutex> lock(m);
    // busco la partida...
    auto it = lobbies.find(matchName);
    if (it == lobbies.end() ||
        it->second.isAvailable() == false) {  // no existe la partida o ya inició
        return false;
    }
    if (it->second.containsPlayer(username) && it->second.isPlayerHost(username)) {
        std::cout << "Inicia Partida." << std::endl;
        auto gameloop = it->second.createGameLoop();
        gameloop->start();
        gameLoops.try_emplace(matchName, std::move(gameloop));
        // lobbies.erase(matchName);
        return true;
    }
    std::cout << "No comenzó la partida porque no es el anfitrión." << std::endl;
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

    // PlayerInfo localPlayerInfo(1, username,Team::CounterTerrorist, PlayerSkin::CounterTerrorist1,
    // Vec2D(20, 20), 0,TypeWeapon::Secondary,100,200,  30); //
    //  TODO: Enviar numPlayers a traves de MatchInfo (ahora hardcodeo).
    return MatchInfo(matchName, ScenarioRegistry::getWindowConfig(),
                     ScenarioRegistry::getTileMap(id_scenary), numPlayers, localPlayerInfo);
}

std::shared_ptr<Queue<PlayerAction>> GameManager::getActionsQueue(const std::string& matchName) {
    std::lock_guard<std::mutex> lock(m);
    return gameLoops.at(matchName)->getActionsQueue();
}

// asegurarse de que solo la clase server pueda llamar a este metodo.
void GameManager::killAllMatchs() {
    std::lock_guard<std::mutex> lock(m);
    for (auto& [matchName, gameLoop]: gameLoops) {
        gameLoop->kill();
        gameLoop->join();
    }
    gameLoops.clear();
}
