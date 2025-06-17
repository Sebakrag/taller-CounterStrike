#ifndef MATCH_H
#define MATCH_H

#include <random>
#include <string>
#include <vector>

#include "../../common/game_info/game_info.h"
#include "../../common/types.h"
#include "weapon/bomb.h"
#include "weapon/projectile.h"

#include "map.h"
#include "physics_engine.h"
#include "player.h"
#include "types2.h"

class Match {
private:
    std::vector<Player> players;
    std::string id_scenario;
    Map map;
    GamePhase phase;
    int roundsPlayed = 0;
    double roundTimer;
    // const double ROUND_DURATION = 120.0;
    bool roundOver = false;
    Team roundWinner;
    std::vector<Projectile> projectiles;
    std::vector<DroppedWeapon> droppedWeapons;
    Bomb bomb;

    // static constexpr int MAX_ROUNDS = 10;
    // static constexpr double PREPARATION_TIME = 30.0;

public:
    explicit Match(const std::string& id_scenario);

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

    // MatchInfo generateMatchInfo() const; (necesitaría recibir el nameMatch en el constructor)
    bool containsPlayer(const std::string& username) const;
    LocalPlayerInfo generateLocalPlayerInfo(const std::string& username) const;
    const std::string& getIdScenario() const;
    int countPlayers() const;

    GameInfo generateGameInfo(const std::string& username) const;

    // void showPlayers() const;
    // std::vector<std::string> getPlayers();

    // metodos privados:
private:
    // void processActionShop(Player* player, const GameAction &gameAction, const float deltaTime);
    // void processActionMatch(Player* player, const GameAction &gameAction, const float deltaTime);
    void handleKnifeAttack(Player* attacker, const Vec2D& direction);


    // CONSTANTES static para inicializar en server.cpp.
private:
    static bool initialized;         // para asegurar que se llame init una unica vez.
    static double ROUND_DURATION;    // duracion máxima de la etapa de combate.
    static double PREPARATION_TIME;  // duracion de la etapa de preparacion.
    static int MAX_ROUNDS;

public:
    // Para cargar los valores del config.yaml
    static void init(double round_duration, double preparation_time, int max_rounds);
};

#endif
