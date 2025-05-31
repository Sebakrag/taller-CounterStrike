#ifndef MATCH_H
#define MATCH_H

#include <string>
#include <vector>

#include "map.h"
#include "player.h"
//#include "game_info.h"
#include "../../common/game_info/game_info.h"
#include "../../common/types.h"
#include "physics_engine.h"

class Match {
private:
    std::vector<Player> players;
    Map map;
    GamePhase phase;
    int roundsPlayed;
    bool bombPlanted;
    int bombPosX, bombPosY;
    double bombTimer;
    const double TIME_TO_EXPLODE = 40.0;
    double roundTimer;
    const double ROUND_DURATION = 120.0;
    bool roundOver;
    Team roundWinner;

public:
    Match();

    void addPlayer(Player&& player);
    bool addPlayer(const std::string& playerName);
    void removePlayer(const std::string& playerName);
    bool movePlayer(const std::string& playerName, const float dx, const float dy, float deltaTime);
    Player* getPlayer(const std::string& playerName);
    void processAction(const PlayerAction& action, const float deltaTime);
    void updateState(double elapsedTime);
    void processPlant(const std::string& playerName);
    void processDefuse(const std::string& playerName);
    void checkRoundEnd();
    GameInfo generateGameInfo(const std::string& playerName) const;
    GameInfo generateGameInfo() const;

    void showPlayers() const;
    std::vector<std::string> getPlayers();
};

#endif
