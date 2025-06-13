#ifndef MATCH_ROOM_H_
#define MATCH_ROOM_H_

#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "gameloop.h"
#include "match.h"
#include "types2.h"

// Acá se van a crear las queues y el gameloop.
// Cuando se cree el gameloop, la instancia de esta clase se debe destruir.

class MatchRoom {
private:
    std::string name_match;
    std::string player_host;
    Match match;
    bool started = false;
    // Archivo del mapa seleccionado
    std::string map_file_name;
    // Por cada jugador: <username, senderQueue>
    std::map<std::string, std::shared_ptr<Queue<GameInfo>>> players;

public:
    MatchRoom(const std::string& name_match, const std::string& username_host,
              std::shared_ptr<Queue<GameInfo>> playerQueue, 
              const std::string& map_file_name = "");

    bool addPlayer(const std::string& username, std::shared_ptr<Queue<GameInfo>> playerQueue);

    void removePlayer(const std::string& username);

    // Crea y devuelve el GameLoop.
    std::shared_ptr<GameLoop> createGameLoop();

    bool isAvailable();

    bool isEmpty();

    bool containsPlayer(const std::string& username) const;

    bool isPlayerHost(const std::string& username) const;

    MatchRoomInfo getMatchRoomInfo();
    
    // Establece el nombre del archivo del mapa
    void setMapFileName(const std::string& fileName);
    
    // Obtiene el nombre del archivo del mapa
    std::string getMapFileName() const;

    // No se puede copiar.
    MatchRoom(const MatchRoom&) = delete;
    MatchRoom& operator=(const MatchRoom&) = delete;


    // metodos static para inicializar constantes.
private:
    static bool initialized;
    static size_t AMOUNT_PLAYERS;

public:
    // Para cargar los valores del config.yaml
    static void init(size_t amountPlayers);
};


#endif  // MATCH_ROOM_H_
