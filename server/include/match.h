#ifndef MATCH_H
#define MATCH_H

#include <vector>
#include "../player.h"
#include "map.h"
#include "action.h"
#include "game_info.h"
#include "../enums/match_phase.h"

class Match {
private:
    std::vector<Player> players;
    Map map;
    MatchPhase phase;
    int roundsPlayed;
    bool bombPlanted;
    int bombPosX, bombPosY;
    double bombTimer;
    const double TIME_TO_EXPLODE = 40.0;
    double roundTimer;
    const double ROUND_DURATION = 120.0;
    bool roundOver;
    PlayerType roundWinner;

public:
    explicit Match();

    void addPlayer(Player&& player);
    bool movePlayer(const int playerId, const int dx, const int dy);
    Player* getPlayer(const int playerId);
    void processAction(const int playerId, const Action& action);
    void updateState(double elapsedTime);
    void processPlant(const int playerId);
    void processDefuse(const int playerId);
    void checkRoundEnd();
    GameInfo generateGameInfo(const int playerId) const;
    void showPlayers() const;
};

#endif
