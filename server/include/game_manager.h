#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "../../common/game_info/game_info.h"
#include "../../common/queue.h"

#include "map_manager.h"
#include "match.h"
#include "match_room.h"

// monitor que gestiona la lista de partidas.
class GameManager {
    std::map<std::string, MatchRoom> lobbies;                    // partidas NO iniciadas.
    std::map<std::string, std::shared_ptr<GameLoop>> gameLoops;  // partidas activas

    std::mutex m;
    bool server_closed = false;
    
    // Gestor de mapas YAML
    MapManager mapManager;

public:
    GameManager(const std::string& mapsDirectory);
    ~GameManager() = default;

    // devuelve true si pudo crear la partida correctamente
    bool createMatch(const std::string& matchName, const std::string& username,
                     std::shared_ptr<Queue<GameInfo>> playerQueue,
                     const std::string& map_file_name = "");

    // devuelve true si pudo unirse a la partida correctamente
    bool JoinMatch(const std::string& matchName, const std::string& username,
                   std::shared_ptr<Queue<GameInfo>> playerQueue);

    // devuelve true si pudo salir de la partida correctamente
    bool QuitMatch(const std::string& matchName, const std::string& username);

    std::list<std::string> listMatchs();

    bool StartMatch(const std::string& username, const std::string& matchName);

    MatchRoomInfo getMatchRoomInfo(const std::string& matchName);

    std::shared_ptr<Queue<PlayerAction>> getActionsQueue(const std::string& matchName);

    void killAllMatchs();

    // Retorna la lista de mapas disponibles
    std::vector<MapInfo> getAvailableMaps() const;
    
    // Retorna el mapa con el nombre de archivo especificado
    TileMap getMap(const std::string& mapFileName) const;

private:
    // void reapDeadGameloops();

    void deleteMatchRoom(const std::string& matchName);
};

#endif  // GAME_MANAGER_H_
