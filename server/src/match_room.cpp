#include "../include/match_room.h"

#include <memory>

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
MatchRoom::MatchRoom(const std::string& name_match, const std::string& username_host):
        name_match(name_match),
        player_host(username_host),
        match(),
        queueActionsPlayers(std::make_shared<Queue<PlayerAction>>()) {
    // Crear la cola para el cliente (GameInfo sender queue)
    players[username_host] = std::make_shared<Queue<GameInfo>>();
}

// Agrega jugador, crea su cola y actualiza el Match
bool MatchRoom::addPlayer(const std::string& username) {
    // Si ya existe, no agrega
    if (players.find(username) != players.end()) {
        return false;
    }

    players[username] = std::make_shared<Queue<GameInfo>>();
    bool addedToMatch = match.addPlayer(username);  // suponiendo addPlayer pública en Match

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

/*std::shared_ptr<GameLoop> MatchRoom::createGameLoop() {
    // Ejemplo básico, asumiendo constructor GameLoop que recibe queues y match
    return std::make_shared<GameLoop>(queueActionsPlayers, players, match);
}
*/

bool MatchRoom::isAvailable() { return players.size() < AMOUNT_PLAYERS; }

bool MatchRoom::isEmpty() { return players.empty(); }
bool MatchRoom::containsPlayer(const std::string& username) const {
    return players.find(username) != players.end();
}
bool MatchRoom::isPlayerHost(const std::string& username) const { return username == player_host; }

// Devuelve un vector con info de los jugadores (asumiendo PlayerInfoLobby tiene constructor o
// setters)
std::vector<PlayerInfoLobby> MatchRoom::getPlayersInRoom() {
    std::vector<PlayerInfoLobby> infos;
    for (const auto& [username, queuePtr]: players) {
        infos.push_back(PlayerInfoLobby(username, Team::Terrorist));  // team hardcodeado
    }
    return infos;
}
