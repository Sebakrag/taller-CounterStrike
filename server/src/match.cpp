#include "../include/match.h"

#include <algorithm>
#include <iostream>
#include <vector>

Match::Match(const TileMap& tilemap):
        map(tilemap), phase(GamePhase::Preparation), roundsPlayed(0) {}

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
                break;
            }

            if (!player->canShoot(0))
                break;

            Vec2D dir = gameAction.direction;
            float norm = std::sqrt(dir.getX() * dir.getX() + dir.getY() * dir.getY());
            if (norm == 0) break;

            float dirX = dir.getX() / norm;
            float dirY = dir.getY() / norm;

            if (player->getEquippedWeapon() == TypeWeapon::Knife) {
                handleKnifeAttack(player, gameAction.direction);
                break;
            }

            std::vector<Projectile> newProjectiles = player->shoot(dirX, dirY, 0);
            projectiles.insert(projectiles.end(), newProjectiles.begin(), newProjectiles.end());
            break;
        }

        case GameActionType::PickUp: {
            for (auto it = droppedWeapons.begin(); it != droppedWeapons.end(); ++it) {
                if (PhysicsEngine::playerTouchingItem(player->getX(), player->getY(), it->position.getX(), it->position.getY())) {
                    //Drop de arma equipada
                    if (player->getPrimaryWeapon()) {
                        droppedWeapons.emplace_back(
                            std::move(player->dropPrimaryWeapon()),
                            Vec2D(player->getX(), player->getY())
                            );
                    }

                    //Pickup arma dropeada
                    player->setPrimaryWeapon(std::move(it->weapon));
                    droppedWeapons.erase(it);
                    break;
                }
            }
            return;
        }

        case GameActionType::Rotate: {
            player->setAngle(action.gameAction.angle);
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


    // Actualizamos los proyectiles
    for (auto& proj : projectiles) {
        if (!proj.isActive()) continue;

        proj.update(static_cast<float>(elapsedTime));

        for (auto& target : players) {
            if (target.getId() == proj.getShooter() || !target.isAlive()) continue;

            float impactDist;
            if (PhysicsEngine::shotHitPlayer(proj.getX(), proj.getY(), proj.getDirX(), proj.getDirY(), map, target, proj.getMaxDistance(), impactDist)) {
                const std::unique_ptr<Weapon_> weapon = WeaponFactory::create(proj.getWeaponUsed());
                target.takeDamage(weapon->getDamage());

                if (!target.isAlive()) {
                    std::unique_ptr<Weapon_> droppedWeapon = target.dropPrimaryWeapon();
                    if (droppedWeapon)
                        droppedWeapons.emplace_back(std::move(droppedWeapon), Vec2D(target.getX(), target.getY()));
                }


                proj.deactivate();

                break;
            }
        }

        //Detectamos colisión con pared
        if (!map.isWalkable(static_cast<int>(proj.getX()), static_cast<int>(proj.getY()))) {
            proj.deactivate();
        }
    }

    //Eliminamos proyectiles inactivos
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
                                            [](const Projectile& p) { return !p.isActive(); }),
                                            projectiles.end());

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

GameInfo Match::generateGameInfo() const {
    std::vector<PlayerInfo> playersInfo;
    unsigned int id = 0;  // temporal. debe ser un atributo de cada objeto
                          // std::cout << "hay " << players.size() << " players" << std::endl;
    for (const auto& p: players) {
        id++;

        PlayerInfo info(id, p.getId(), p.getTeam(), PlayerSkin::CounterTerrorist1,
                        Vec2D(p.getX(), p.getY()), p.getAngle(), p.getEquippedWeapon(),
                        p.getHealth(), static_cast<int>(p.getMoney()),
                        p.getPrimaryWeapon() ? p.getPrimaryWeapon()->getBullets() : 0);
        playersInfo.push_back(info);
    }

    std::vector<ProjectileInfo> projectileInfos;
    for (const auto& p : projectiles) {
        projectileInfos.emplace_back(
            p.getX(),
            p.getY(),
            p.getDirX(),
            p.getDirY(),
            p.getShooter(),
            p.getWeaponUsed(),
            p.isActive()
        );
    }

    GameInfo gameInfo(this->phase, roundTimer, playersInfo, projectileInfos);
    return gameInfo;
}

void Match::showPlayers() const {
    std::cout << "Players in match:\n";
    for (const auto& p: players) {
        std::cout << " - Player " << p.getId() << " en (" << p.getX() << "," << p.getY() << ") "
                  << (p.isAlive() ? "[VIVO]" : "[MUERTO]") << "\n";
    }
}

void Match::handleKnifeAttack(Player *attacker, const Vec2D &direction) {
    for (auto& target : players) {
        if (target.getId() == attacker->getId() || !target.isAlive()) continue;

        float impactDistance;
        if (PhysicsEngine::knifeHit(attacker->getX(), attacker->getY(), direction.getX(), direction.getY(), target, impactDistance)) {
            target.takeDamage(20);
        }
    }
}

