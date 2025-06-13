
#include "../include/game_manager.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <filesystem>

GameManager::GameManager(const std::string& mapsDirectory) : mapManager(mapsDirectory) {
    // Inicializar el MapManager con la carpeta de mapas
    try {
        std::cout << "Inicializando MapManager con directorio: " << mapsDirectory << std::endl;
        if (!std::filesystem::exists(mapsDirectory)) {
            std::cout << "El directorio de mapas no existe. Creando directorio..." << std::endl;
            std::filesystem::create_directories(mapsDirectory);
        }
        // El mapManager ya fue inicializado con el directorio en la lista de inicialización
    } catch (const std::exception& e) {
        std::cerr << "Error al inicializar el MapManager: " << e.what() << std::endl;
    }
}
bool GameManager::createMatch(const std::string& matchName, const std::string& username,
                              std::shared_ptr<Queue<GameInfo>> playerQueue,
                              const std::string& map_file_name) {
    // chequeo que no sea un nombre sin caracteres
    if (matchName.empty() || std::all_of(matchName.begin(), matchName.end(), ::isspace)) {
        return false;
    }
    std::lock_guard<std::mutex> lock(m);

    if (server_closed || lobbies.find(matchName) != lobbies.end()) {  // si la partida ya existe
        return false;
    }
    
    // Validar que el archivo del mapa exista en el MapManager si se proporciona uno
    if (!map_file_name.empty()) {
        try {
            // Verificar que el mapa existe
            bool mapExists = false;
            auto maps = mapManager.getAvailableMaps();
            for (const auto& map : maps) {
                if (map.fileName == map_file_name) {
                    mapExists = true;
                    break;
                }
            }
            
            if (!mapExists) {
                std::cerr << "El mapa '" << map_file_name << "' no existe en el directorio de mapas." << std::endl;
                return false;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error al verificar el mapa: " << e.what() << std::endl;
            return false;
        }
    }
    
    // Crear la sala con el mapa especificado
    lobbies.try_emplace(matchName, matchName, username, playerQueue, map_file_name);

    std::cout << username << " creó la partida " << matchName;
    if (!map_file_name.empty()) {
        std::cout << " con el mapa '" << map_file_name << "'";
    }
    std::cout << std::endl;

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

std::shared_ptr<Queue<PlayerAction>> GameManager::getActionsQueue(const std::string& matchName) {
    std::lock_guard<std::mutex> lock(m);
    return gameLoops.at(matchName)->getActionsQueue();
}

void GameManager::killAllMatchs() {
    std::lock_guard<std::mutex> lock(m);
    for (auto& [matchName, gameLoop]: gameLoops) {
        gameLoop->kill();
        gameLoop->join();
    }
    gameLoops.clear();
}

std::vector<MapInfo> GameManager::getAvailableMaps() const {
    return mapManager.getAvailableMaps();
}

TileMap GameManager::getMap(const std::string& mapFileName) const {
    try {
        return mapManager.getTileMap(mapFileName);
    } catch (const std::exception& e) {
        std::cerr << "Error al obtener el mapa " << mapFileName << ": " << e.what() << std::endl;
        // Devolver un mapa predeterminado en caso de error
        return TileMap::getCurrentLevel();
    }
}
