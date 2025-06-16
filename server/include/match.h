#ifndef MATCH_H
#define MATCH_H

#include <string>
#include <vector>
#include <random>

#include "map.h"
#include "player.h"
#include "weapon/bomb.h"
#include "../../common/game_info/game_info.h"
#include "../../common/types.h"
#include "weapon/projectile.h"
#include "types2.h"
#include "shop.h"

#include "physics_engine.h"

class Match {
private:
    std::vector<Player> players;
    Map map;
    GamePhase phase;
    int roundsPlayed = 0;
    double roundTimer = 30.0;
    const double ROUND_DURATION = 120.0;
    bool roundOver;
    Team roundWinner;
    std::vector<Projectile> projectiles;
    std::vector<DroppedWeapon> droppedWeapons;
    Bomb bomb;

    static constexpr int MAX_ROUNDS = 10;
    static constexpr double PREPARATION_TIME = 30.0;
    static constexpr float BASE_MONEY_BONUS = 300.0f;
    static constexpr float KILL_BONUS = 10.0f;
    static constexpr float WIN_BONUS = 100.0f;

public:
    explicit Match(const TileMap& tilemap);

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
    void advancePhase();
    GamePhase getGamePhase() const;

    GameInfo generateGameInfo() const;
    MatchInfo generateMatchInfo() const;

    void showPlayers() const;
    std::vector<std::string> getPlayers();
    void handleKnifeAttack(Player* attacker, const Vec2D& direction);
};

#endif
