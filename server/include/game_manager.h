#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "../../common/game_info.h"
#include "../../common/queue.h"

#include "match.h"
#include "match_room.h"

// monitor que gestiona la lista de partidas.
class GameManager {
    std::map<std::string, MatchRoom> lobbies;                    // partidas NO iniciadas.
    std::map<std::string, std::shared_ptr<GameLoop>> gameLoops;  // partidas activas

    std::mutex m;

    bool server_closed = false;

public:
    //    GameManager(lista de escenarios, configuraciones, etc) // redefinir el constructor despues

    // devuelve true si pudo crear la partida correctamente
    bool createMatch(const std::string& matchName, const std::string& username,
                     std::shared_ptr<Queue<GameInfo>> playerQueue);

    // devuelve true si pudo unirse a la partida correctamente
    bool JoinMatch(const std::string& matchName, const std::string& username,
                   std::shared_ptr<Queue<GameInfo>> playerQueue);

    // devuelve true si pudo salir de la partida correctamente
    bool QuitMatch(const std::string& matchName, const std::string& username);

    std::list<std::string> listMatchs();

    bool startMatch(const std::string& username, const std::string& matchName);

    std::vector<PlayerInfoLobby> getPlayersInMatch(const std::string& matchName);

    std::shared_ptr<Queue<PlayerAction>> getActionsQueue(const std::string& matchName);

    void killAllMatchs();

private:
    // void reapDeadGameloops();

    void deleteMatchRoom(const std::string& matchName);
};

#endif  // GAME_MANAGER_H_
