#include "../include/match_room.h"

#include <memory>
#include <stdexcept>
#include <utility>

#include "../include/scenario_registry.h"

// Inicializo las variables estáticas (para poder compilar)
bool MatchRoom::initialized = false;
size_t MatchRoom::MAX_PLAYERS = 0;

void MatchRoom::init(size_t max_players) {
    if (initialized == false) {
        MAX_PLAYERS = max_players;
        initialized = true;
    }
}

// Constructor: inicializa nombre, host, crea Match y cola de acciones
MatchRoom::MatchRoom(const std::string& name_match, const std::string& username_host,
                     std::shared_ptr<Queue<GameInfo>> playerQueue, const std::string& id_scenario):
        name_match(name_match),
        player_host(username_host),
        match(id_scenario),
        id_scenario(id_scenario) {
    addPlayer(username_host, playerQueue);
}

// Agrega jugador, crea su cola y actualiza el Match
bool MatchRoom::addPlayer(const std::string& username,
                          std::shared_ptr<Queue<GameInfo>> playerQueue) {
    // Si ya existe, no agrega
    if (players.find(username) != players.end() || !isAvailable()) {
        return false;
    }

    players[username] = playerQueue;
    bool addedToMatch = match.addPlayer(username);

    return addedToMatch;
}

// Remueve jugador y su cola, y actualiza el Match
void MatchRoom::removePlayer(const std::string& username) {
    players.erase(username);
    match.removePlayer(username);
    if (players.empty()) {
        return;
    }
    if (username == player_host) {
        player_host = players.begin()->first;
        // decidir.
        // Si elijo poner a otro como host debería mandarle un mensaje por el protocolo.
        // si elijo borrar la partida, debería mandarle un mensaje a todos de que se canceló.
    }
}

bool MatchRoom::isAvailable() { return players.size() < MAX_PLAYERS && !started; }

bool MatchRoom::isEmpty() { return players.empty(); }

bool MatchRoom::containsPlayer(const std::string& username) const {
    return players.find(username) != players.end();
}

bool MatchRoom::isPlayerHost(const std::string& username) const { return username == player_host; }

MatchRoomInfo MatchRoom::getMatchRoomInfo() {
    std::vector<PlayerInfoLobby> infos;
    for (const auto& [username, queuePtr]: players) {
        auto p = match.getPlayer(username);
        if (p == nullptr) {
            throw std::runtime_error("ERROR en MatchRoom::getMatchRoomInfo()");
        }
        bool is_host = (p->getId() == player_host);
        infos.push_back(PlayerInfoLobby(username, p->getTeam(), is_host));
    }

    return MatchRoomInfo(infos, started);
}

std::shared_ptr<GameLoop> MatchRoom::createGameLoop() {
    started = true;
    return std::make_shared<GameLoop>(match, players);
}
