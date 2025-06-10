#ifndef MATCH_H
#define MATCH_H

#include <string>
#include <vector>

#include "map.h"
#include "player.h"
#include "../../common/game_info/game_info.h"
#include "../../common/types.h"
#include "weapon/projectile.h"
#include "types2.h"

#include "physics_engine.h"

class Match {
private:
    std::vector<Player> players;
    Map map;
    GamePhase phase;
    int roundsPlayed = 0;
    bool bombPlanted = false;
    int bombPosX, bombPosY;
    double bombTimer;
    const double TIME_TO_EXPLODE = 40.0;
    double roundTimer = 1.0;
    const double ROUND_DURATION = 120.0;
    bool roundOver = false;
    Team roundWinner;
    std::vector<Projectile> projectiles;
    std::vector<DroppedWeapon> droppedWeapons;

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
    void handleKnifeAttack(Player* attacker, const Vec2D& direction);
};

#endif
