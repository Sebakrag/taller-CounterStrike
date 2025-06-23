#ifndef MATCH_H
#define MATCH_H

#include <random>
#include <string>
#include <vector>
#include <list>

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
    std::list<DroppedWeapon> droppedWeapons;
    Bomb bomb;

    double current_time = 0;  // (en ms) se incrementa en cada update


public:
    explicit Match(const std::string& id_scenario);

    bool addPlayer(const std::string& playerName);
    void removePlayer(const std::string& playerName);

    Player* getPlayer(const std::string& playerName);  // este metodo está raro que sea publico
    GamePhase getGamePhase() const;
    void processAction(const PlayerAction& action, const float deltaTime);
    void updateState(double elapsedTime);

    // MatchInfo generateMatchInfo() const; (necesitaría recibir el nameMatch en el constructor. me
    // ahorro los 4 metodos)
    bool containsPlayer(const std::string& username) const;
    LocalPlayerInfo generateLocalPlayerInfo(const std::string& username) const;
    const std::string& getIdScenario() const;
    int countPlayers() const;

    GameInfo generateGameInfo(const std::string& username) const;

    void resetStatesOfPlayers();  // si no se nos ocurre algo mejor. Está bien que lo haga el
                                  // gameloop?
    // void showPlayers() const;
    // std::vector<std::string> getPlayers();

    // metodos privados:
private:
    bool movePlayer(const std::string& playerName, const float dx, const float dy, float deltaTime);
    void processPlant(const std::string& playerName);
    void processDefuse(const std::string& playerName);

    void checkRoundEnd();
    void advancePhase();
    // void processActionShop(Player* player, const GameAction &gameAction, const float deltaTime);
    // void processActionMatch(Player* player, const GameAction &gameAction, const float deltaTime);
    void handleKnifeAttack(Player* attacker, const Vec2D& direction);
    void setPosSpawnPlayer(Player& p);
    bool isFriendlyFire(const std::string& shooterId, Team targetTeam) const;


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
