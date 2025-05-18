#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

#include <list>
#include <map>
#include <mutex>
#include <string>
#include <vector>

#include "match.h"


// monitor que gestiona la lista de partidas.
class GameManager {
    // std::map<std::string , Match> matchs;
    std::list<std::string> matchs;  // CAMBIAR
    std::mutex m;

public:
    //    GameManager(lista de escenarios)
    // devuelve true si pudo crear la partida correctamente
    bool createMatch(const std::string& matchName, const std::string& username);

    // devuelve true si pudo unirse a la partida correctamente
    bool JoinMatch(const std::string& matchName, const std::string& username);

    // borra la partida. solo el usuario creador puede hacerlo.
    bool deleteMatch(const std::string& matchName, const std::string& username);

    std::list<std::string> listMatchs();


private:
    // void reapMatchs();
};

#endif  // GAME_MANAGER_H_
