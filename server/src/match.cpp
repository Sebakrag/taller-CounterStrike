#include "../include/match.h"

#include <algorithm>
#include <iostream>
#include <vector>

Match::Match(): phase(GamePhase::Preparation), roundsPlayed(0) {}

void Match::addPlayer(Player&& player) { players.emplace_back(std::move(player)); }

bool Match::addPlayer(const std::string& username) {
    int terroristCount = 0;
    int counterTerroristCount = 0;

    for (const auto& p: players) {
        if (p.getTeam() == Team::Terrorist)
            terroristCount++;
        else if (p.getTeam() == Team::CounterTerrorist)
            counterTerroristCount++;
    }

    Team assignedTeam =
            (terroristCount <= counterTerroristCount) ? Team::Terrorist : Team::CounterTerrorist;

    Player newPlayer(username, assignedTeam);
    players.push_back(std::move(newPlayer));
    return true;
}

void Match::removePlayer(const std::string& username) {
    for (auto& p: players) {
        if (p.getId() == username) {
            // players.erase(std::find(players.begin(), players.end(), p));
        }
    }
}


// std::vector<std::string> Match::getPlayers() { return players; }


Player* Match::getPlayer(const std::string& playerName) {
    for (auto& p: players) {
        if (p.getId() == playerName)
            return &p;
    }
    return nullptr;
}

bool Match::movePlayer(const std::string& playerName, const float dx, const float dy,
                       float deltaTime) {
    Player* p = getPlayer(playerName);
    if (!p || !p->isAlive())
        return false;

    try {
        std::cout << "En match::movePlayer llega con los parametros: (" << dx << ", " << dy
                  << "), y deltatime: " << deltaTime << std::endl;
        PhysicsEngine::movePlayer(*p, dx, dy, deltaTime, map);
        return true;
    } catch (...) {
        return false;
    }
}
void Match::processAction(const PlayerAction& action, const float deltaTime) {
    Player* player = getPlayer(action.player_username);
    if (!player || !player->isAlive())
        return;

    GameAction gameAction = action.gameAction;
    switch (gameAction.type) {
        case GameActionType::Walk: {
            const float dx = gameAction.direction.getX();
            const float dy = gameAction.direction.getY();
            movePlayer(action.player_username, dx, dy, deltaTime);
            break;
        }
        case GameActionType::ChangeWeapon: {
            player->setEquippedWeapon(gameAction.typeWeapon);
            break;
        }
        case GameActionType::Attack: {
            if (player->getEquippedWeapon() == TypeWeapon::Bomb) {
                processPlant(action.player_username);
            }
            const int dmg =
                    player->attack(gameAction.direction.getX(), gameAction.direction.getY());
            std::cout << "Player " << action.player_username << " shoot with " << dmg
                      << " damage\n";
            break;
        }
        // case ActionType::DEFUSE:
        //   processDefuse(playerId);
        //   break;
        default:
            std::cout << "Accion no implementada\n";
            break;
    }
}

void Match::updateState(double elapsedTime) {
    if (roundOver)
        return;

    roundTimer -= elapsedTime;
    if (bombPlanted) {
        bombTimer -= elapsedTime;
        if (bombTimer <= 0) {
            std::cout << "La bomba exploto!\n";
            roundOver = true;
            roundWinner = Team::Terrorist;
        }
    }

    checkRoundEnd();
}


void Match::processPlant(const std::string& playerName) {
    Player* player = getPlayer(playerName);
    if (!player || !player->isAlive() || player->getTeam() != Team::Terrorist)
        return;
    if (bombPlanted)
        return;

    int x = player->getX();
    int y = player->getY();
    // if (!map.isBombSite(x, y))
    //   return;

    bombPlanted = true;
    bombPosX = x;
    bombPosY = y;
    bombTimer = TIME_TO_EXPLODE;

    std::cout << "Player " << playerName << " planted the bomb at (" << x << ", " << y << ")\n";
}

void Match::processDefuse(const std::string& playerName) {
    Player* player = getPlayer(playerName);
    if (!player || !player->isAlive() || player->getTeam() != Team::CounterTerrorist)
        return;
    if (!bombPlanted)
        return;

    int x = player->getX();
    int y = player->getY();
    if (x != bombPosX || y != bombPosY)
        return;

    bombPlanted = false;
    bombTimer = 0;
    roundOver = true;
    roundWinner = Team::CounterTerrorist;

    std::cout << "Player " << playerName << " defused the bomb!\n";
}

void Match::checkRoundEnd() {
    if (roundOver)
        return;

    bool terroristsLeft = false;
    bool antiterroristsLeft = false;

    for (const auto& p: players) {
        if (!p.isAlive())
            continue;
        if (p.getTeam() == Team::Terrorist)
            terroristsLeft = true;
        if (p.getTeam() == Team::CounterTerrorist)
            antiterroristsLeft = true;
    }

    if (!terroristsLeft) {
        roundOver = true;
        roundWinner = Team::CounterTerrorist;
        std::cout << "Todos los terroristas murieron. Ganan los antiterroristas. \n";
    } else if (!antiterroristsLeft) {
        roundOver = true;
        roundWinner = Team::Terrorist;
        std::cout << "Todos los antiterroristas murieron. Ganan los terroristas. \n";
    }
    // else if (roundTimer <= 0 && !bombPlanted) {
    //     roundOver = true;
    //     roundWinner = Team::CounterTerrorist;
    //     std::cout << "Se acabó el tiempo sin bomba. Ganan los antiterroristas. \n";
    // }
}
/*
GameInfo Match::generateGameInfo(const std::string& playerName) const {
    GameInfo info;

    for (const auto& p : players) {
        if (p.getId() == playerName) {
            info.posX = p.getX();
            info.posY = p.getY();
            info.health = p.getHealth();
            info.equippedWeapon = p.getEquippedWeapon();
            info.bullets = p.getEquippedWeapon() == WeaponType::PRIMARY && p.getPrimaryWeapon() ?
                           p.getPrimaryWeapon()->getBullets() :
                           (p.getEquippedWeapon() == WeaponType::SECONDARY && p.getSecondaryWeapon()
? p.getSecondaryWeapon()->getBullets() : 0); } else { info.otherPlayers.push_back(PlayerInfo{
                p.getId(), p.getX(), p.getY(), p.isAlive(), p.getType()
            });
        }
    }

    info.bombPlanted = bombPlanted;
    info.bombX = bombPosX;
    info.bombY = bombPosY;
    info.timeLeft = roundTimer;

    return info;
}
*/
GameInfo Match::generateGameInfo() const {
    std::vector<PlayerInfo> playersInfo;
    unsigned int id = 0;  // temporal. debe ser un atributo de cada objeto
                          // std::cout << "hay " << players.size() << " players" << std::endl;
    for (const auto& p: players) {
        id++;

        PlayerInfo info(id, p.getId(), p.getTeam(), PlayerSkin::CounterTerrorist1,
                        Vec2D(p.getX(), p.getY()), 0, p.getEquippedWeapon(), p.getHealth(),
                        static_cast<int>(p.getMoney()),
                        p.getPrimaryWeapon() ? p.getPrimaryWeapon()->getBullets() : 0);
        playersInfo.push_back(info);
    }

    // Crear un vector vacío de ProjectileInfo para el cuarto parámetro requerido
    std::vector<ProjectileInfo> projectiles;
    
    // Ahora usamos el constructor con los 4 parámetros necesarios
    GameInfo gameInfo(this->phase, static_cast<float>(roundTimer), playersInfo, projectiles);
    return gameInfo;
}

void Match::showPlayers() const {
    std::cout << "Players in match:\n";
    for (const auto& p: players) {
        std::cout << " - Player " << p.getId() << " en (" << p.getX() << "," << p.getY() << ") "
                  << (p.isAlive() ? "[VIVO]" : "[MUERTO]") << "\n";
    }
}
