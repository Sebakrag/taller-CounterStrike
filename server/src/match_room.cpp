#include "../include/match_room.h"

#include <memory>
#include <utility>

// Inicializo las variables estáticas (para poder compilar)
bool MatchRoom::initialized = false;
size_t MatchRoom::AMOUNT_PLAYERS = 0;

void MatchRoom::init(size_t amountPlayers) {
    if (initialized == false) {
        AMOUNT_PLAYERS = amountPlayers;
        initialized = true;
    }
}

// Constructor: inicializa nombre, host, crea Match y cola de acciones
MatchRoom::MatchRoom(const std::string& name_match, const std::string& username_host,
                     std::shared_ptr<Queue<GameInfo>> playerQueue):
        name_match(name_match), player_host(username_host), match() {
    // Crear la cola para el cliente (GameInfo sender queue)
    players[username_host] = playerQueue;
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
    if (username == player_host) {
        // decidir.
        // Si elijo poner a otro como host debería mandarle un mensaje por el protocolo.
        // si elijo borrar la partida, debería mandarle un mensaje a todos de que se canceló.
    }
}

bool MatchRoom::isAvailable() { return players.size() < AMOUNT_PLAYERS && !started; }

bool MatchRoom::isEmpty() { return players.empty(); }

bool MatchRoom::containsPlayer(const std::string& username) const {
    return players.find(username) != players.end();
}

bool MatchRoom::isPlayerHost(const std::string& username) const { return username == player_host; }

MatchRoomInfo MatchRoom::getMatchRoomInfo() {
    std::vector<PlayerInfoLobby> infos;
    for (const auto& [username, queuePtr]: players) {
        infos.push_back(PlayerInfoLobby(username, Team::Terrorist));  // team hardcodeado
    }

    return MatchRoomInfo(infos, started);
}

std::shared_ptr<GameLoop> MatchRoom::createGameLoop() {
    std::list<std::shared_ptr<Queue<GameInfo>>> playerQueues;

    for (const auto& pair: players) {
        playerQueues.push_back(pair.second);
    }
    started = true;
    return std::make_shared<GameLoop>(std::move(match), playerQueues);
}
