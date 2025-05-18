
#include "../include/game_manager.h"

#include <algorithm>

/*GameManager::GameManager(const std::list<std::string>& escenarios) {
    // YOUR CODE
}
*/
bool GameManager::createMatch(const std::string& matchName, const std::string& username) {
    std::lock_guard<std::mutex> lock(m);

    if (server_closed || matchs.find(matchName) != matchs.end()) {  // si la partida ya existe
        return false;
    }
    matchs.try_emplace(matchName, std::make_shared<Match>(username));

    std::cout << username << " creó la partida " << matchName << std::endl;

    return true;
}

bool GameManager::JoinMatch(const std::string& matchName, const std::string& username) {
    std::lock_guard<std::mutex> lock(m);

    if (server_closed)
        return false;
    auto it = matchs.find(matchName);
    if (it == matchs.end() ||
        it->second->isAvailable() == false) {  // no existe la partida o ya inició
        return false;
    }

    it->second->addPlayer(username);
    std::cout << username << " se unió a partida " << matchName << std::endl;
    return true;
}

bool GameManager::deleteMatch(const std::string& matchName, const std::string& username) {
    std::lock_guard<std::mutex> lock(m);
    // YOUR CODE

    std::cout << matchName << std::endl;
    std::cout << username << std::endl;
    return false;
}

std::list<std::string> GameManager::listMatchs() {
    std::lock_guard<std::mutex> lock(m);
    // YOUR CODE
    std::list<std::string> sorted_matches;
    for (const auto& match: matchs) {
        sorted_matches.push_back(match.first);
    }
    sorted_matches.sort();
    return sorted_matches;
}

std::shared_ptr<Match> GameManager::getMatch(const std::string& matchName,
                                             const std::string& username) {
    std::lock_guard<std::mutex> lock(m);

    auto it = matchs.find(matchName);
    if (it == matchs.end()) {
        throw std::runtime_error("La partida no existe");
    }

    // Verificar si el usuario está en la partida
    std::vector<std::string> players = it->second->getPlayers();
    if (std::find(players.begin(), players.end(), username) == players.end()) {
        throw std::runtime_error("El usuario no está en la partida");
    }

    return it->second;
}
/*void GameManager::reapMatchs() {
    std::lock_guard<std::mutex> lock(m);
    // YOUR CODE
}
*/
