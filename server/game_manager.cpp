
#include "game_manager.h"

/*GameManager::GameManager(const std::list<std::string>& escenarios) {
    // YOUR CODE
}
*/
bool GameManager::createMatch(const std::string& matchName, const std::string& username) {
    std::lock_guard<std::mutex> lock(m);

    for (const auto& match: matchs) {
        if (match == matchName) {
            return false;
        }
    }
    std::cout << username << " creó la partida " << matchName << std::endl;
    matchs.push_back(matchName);
    // YOUR CODE

    return true;
}

bool GameManager::JoinMatch(const std::string& matchName, const std::string& username) {
    std::lock_guard<std::mutex> lock(m);

    bool matchFound = false;
    for (const auto& match: matchs) {
        if (match == matchName) {
            matchFound = true;
            break;
        }
    }
    if (matchFound) {
        std::cout << username << " se unió a partida " << matchName << std::endl;
        return true;
    }
    // YOUR CODE

    return false;
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
    std::list<std::string> sorted_matches = matchs;
    sorted_matches.sort();
    return sorted_matches;
}

/*void GameManager::reapMatchs() {
    std::lock_guard<std::mutex> lock(m);
    // YOUR CODE
}
*/
