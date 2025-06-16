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

    std::cout << "Agregando jugador " << username << " al equipo " << (assignedTeam == Team::Terrorist ? " Terrorista" : "Antiterrorista") << std::endl;
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
    if (phase == GamePhase::Combat) {
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

                if (bomb.isDropped() &&
                    PhysicsEngine::playerTouchingItem(player->getX(), player->getY(), bomb.getX(), bomb.getY())) {
                    if (player->getTeam() == Team::Terrorist) {
                        bomb.pickUp(player->getId());
                        std::cout << "Jugador " << player->getId() << " agarro la bomba. \n";
                    }
                    }
                return;
            }

            case GameActionType::Rotate: {
                player->setAngle(action.gameAction.angle);
                break;
            }
            case GameActionType::PlantBomb: {
                processPlant(action.player_username);
                break;
            }
            case GameActionType::DefuseBomb: {
                processDefuse(action.player_username);
                break;
            }
            default:
                std::cout << "Accion no implementada\n";
                break;
        }
    }
}

void Match::updateState(double elapsedTime) {
    //std::cout << "UpdateState " << roundTimer << " : Juego en fase " << (phase == GamePhase::Preparation ? "Preparation " : "Combate ") << std::endl;
    if (roundOver)
        return;

    roundTimer -= elapsedTime;

    if (phase == GamePhase::Combat && roundTimer <= 0 && !bomb.isPlanted()) {
        std::cout << "Se acabo el tiempo sin plantar la bomba. Ganan los antiterroristas\n";
        roundOver = true;
        roundWinner = Team::CounterTerrorist;
    }

    bomb.update(elapsedTime);
    if (bomb.hasExploded()) {
        std::cout << "La bomba exploto!\n";
        roundOver = true;
        roundWinner = Team::Terrorist;
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
                    if (bomb.isCarriedBy(target.getId())) {
                        bomb.drop(target.getX(), target.getY());
                    }
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

    if (roundOver || roundTimer <= 0) {
        std::cout << "Avanzamos de fase\n";
        advancePhase();
    }
}


void Match::processPlant(const std::string& playerName) {
    Player* player = getPlayer(playerName);
    if (!player || !player->isAlive() || player->getTeam() != Team::Terrorist)
        return;

    if (!bomb.isCarriedBy(playerName))
        return;

    if (bomb.plant(player->getX(), player->getY(), map)) {
        std::cout << "Player " << playerName << " planted the bomb at (" << player->getX() << ", " << player->getY() << ")\n";
    }
}

void Match::processDefuse(const std::string& playerName) {
    Player* player = getPlayer(playerName);
    if (!player || !player->isAlive() || player->getTeam() != Team::CounterTerrorist)
        return;

    if (bomb.isPlanted() &&
        PhysicsEngine::playerTouchingItem(player->getX(), player->getY(), bomb.getX(), bomb.getY())) {
        if (bomb.defuse()) {
            std::cout << "Player " << playerName << " defused the bomb!\n";
            roundOver = true;
            roundWinner = Team::CounterTerrorist;
        }
    }
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
        if (bomb.isPlanted()) {
            std::cout << "Todos los terroristas murieron pero la bomba esta plantada. Se espera desactivacion...\n";
            return;
        }
        roundOver = true;
        roundWinner = Team::CounterTerrorist;
        std::cout << "Todos los terroristas murieron. Ganan los antiterroristas. \n";
    } else if (!antiterroristsLeft) {
        roundOver = true;
        roundWinner = Team::Terrorist;
        std::cout << "Todos los antiterroristas murieron. Ganan los terroristas. \n";
    }
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

void Match::advancePhase() {
    if (phase == GamePhase::Preparation) {
        std::cout << "==> INICIA RONDA " << roundsPlayed + 1 << "\n";

        //Limpieza entre rondas
        bomb.reset();
        projectiles.clear();
        droppedWeapons.clear();

        for (auto& p: players) {
            p.revive();
            p.setEquippedWeapon(TypeWeapon::Knife);
        }

        //Asignar la bomba a un terrorista vivo
        std::vector<Player*> terrorists;
        for (auto& p: players) {
            if (p.getTeam() == Team::Terrorist)
                terrorists.push_back(&p);
        }

        if (!terrorists.empty()) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, terrorists.size() - 1);
            Player* bombCarrier = terrorists[dis(gen)];
            bomb.assignTo(bombCarrier->getId());
            std::cout << "La bomba fue asignada a: " << bombCarrier->getId() << "\n";
        }

        roundOver = false;
        roundTimer = ROUND_DURATION;
        phase = GamePhase::Combat;
    } else if (phase == GamePhase::Combat) {
        roundsPlayed++;

        std::cout << "==> TERMINO LA RONDA " << roundsPlayed << "\n";

        //Cambio de lados a la mitad
        if (roundsPlayed == MAX_ROUNDS / 2) {
            std::cout << "==> Cambio de lados!\n";
            for (auto& p: players) {
                Team newTeam = (p.getTeam() == Team::Terrorist) ? Team::CounterTerrorist : Team::Terrorist;
                p.setTeam(newTeam);
            }
        }

        //Fin de la partida
        if (roundsPlayed >= MAX_ROUNDS) {
            std::cout << "==> PARTIDA TERMINADA\n";
            phase = GamePhase::EndOfMatch;
            return;
        }

        //Pasar a la fase de preparacion
        roundTimer = PREPARATION_TIME;
        phase = GamePhase::Preparation;
    }
}

GamePhase Match::getGamePhase() const {
    return phase;
}


