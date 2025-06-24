#include "../include/match.h"

#include <algorithm>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <vector>

#include "../include/scenario_registry.h"

// Inicializo las variables estáticas (para poder compilar)
bool Match::initialized = false;
double Match::ROUND_DURATION = 0;
double Match::PREPARATION_TIME = 0;
int Match::MAX_ROUNDS = 0;

void Match::init(double round_duration, double preparation_time, int max_rounds) {
    if (initialized == false) {
        ROUND_DURATION = round_duration;
        PREPARATION_TIME = preparation_time;
        MAX_ROUNDS = max_rounds;
        initialized = true;
    }
}

Match::Match(const std::string& id_scenario):
        id_scenario(id_scenario),
        map(ScenarioRegistry::getTileMap(id_scenario)),
        phase(GamePhase::Preparation),
        roundsPlayed(0),
        roundTimer(PREPARATION_TIME) {
    // TODO: Harcodeo un dropped weapon para probar. Eliminarlo.
    // auto ak47 = std::make_unique<WeaponAk47>();
    // droppedWeapons.emplace_back(DroppedWeapon{std::move(ak47), {300, 300}});
    // auto m3 = std::make_unique<WeaponM3>();
    // droppedWeapons.emplace_back(DroppedWeapon{std::move(m3), {400, 350}});
    // auto awp = std::make_unique<WeaponAwp>();
    // droppedWeapons.emplace_back(DroppedWeapon{std::move(awp), {600, 380}});
    // auto bomb = std::make_unique<Bomb>();
    // droppedWeapons.emplace_back(DroppedWeapon{std::move(bomb), {300, 300}});

    const auto& weaponPositions = map.getWeaponPositions();
    for (const auto& weaponPair: weaponPositions) {
        const Vec2D& position = weaponPair.first;
        Weapon weaponType = weaponPair.second;

        switch (weaponType) {
            case Weapon::Ak47: {
                auto ak47 = std::make_unique<WeaponAk47>();
                droppedWeapons.emplace_back(DroppedWeapon{std::move(ak47), position});
                break;
            }
            case Weapon::M3: {
                auto m3 = std::make_unique<WeaponM3>();
                droppedWeapons.emplace_back(DroppedWeapon{std::move(m3), position});
                break;
            }
            case Weapon::Awp: {
                auto awp = std::make_unique<WeaponAwp>();
                droppedWeapons.emplace_back(DroppedWeapon{std::move(awp), position});
                break;
            }
            case Weapon::Glock: {
                auto glock = std::make_unique<WeaponGlock>();
                droppedWeapons.emplace_back(DroppedWeapon{std::move(glock), position});
                break;
            }
            default:
                break;
        }
    }
}

// void Match::addPlayer(Player&& player) { players.emplace_back(std::move(player)); }

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
    setPosSpawnPlayer(newPlayer);
    players.push_back(std::move(newPlayer));
    return true;
}

void Match::removePlayer(const std::string& username) {
    for (auto it = players.begin(); it != players.end(); ++it) {
        if (it->getId() == username) {
            players.erase(it);
            return;
        }
    }
}

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
        PhysicsEngine::movePlayer(*p, dx, dy, deltaTime, map);
        return true;
    } catch (...) {
        return false;
    }
}

void Match::processAction(const PlayerAction& action, const float deltaTime) {
    if (phase == GamePhase::Preparation) {
        // processActionShop(const PlayerAction& action, const float deltaTime)
    } else if (phase == GamePhase::Combat) {
        // precessActionMatch(const PlayerAction& action, const float deltaTime)
    } else {  // end of the match
        return;
    }
    Player* player = getPlayer(action.player_username);
    if (!player || !player->isAlive())
        return;
    // player->setState(PlayerState::Idle);

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

                // if (!player->canShoot(current_time)) { ya se chequea en player->shoot()
                // internamente
                //     std::cout << "el player no puede disparar" << std::endl;
                //     break;
                // }

                Vec2D dir = gameAction.direction;
                float norm = std::sqrt(dir.getX() * dir.getX() + dir.getY() * dir.getY());
                if (norm == 0) {
                    break;
                }

                float dirX = dir.getX() / norm;
                float dirY = dir.getY() / norm;

                if (player->getEquippedWeapon() == TypeWeapon::Knife) {
                    player->setState(PlayerState::Attacking);
                    handleKnifeAttack(player, gameAction.direction);
                    break;
                }

                std::vector<Projectile> newProjectiles = player->shoot(dirX, dirY, current_time);
                projectiles.insert(projectiles.end(), newProjectiles.begin(), newProjectiles.end());
                break;
            }

            case GameActionType::PickUp: {
                for (auto it = droppedWeapons.begin(); it != droppedWeapons.end(); ++it) {
                    if (!it->weapon)
                        continue;
                    if (PhysicsEngine::playerTouchingItem(player->getX(), player->getY(),
                                                          it->position.getX(),
                                                          it->position.getY())) {
                        // Drop de arma equipada
                        if (player->getPrimaryWeapon()) {
                            std::unique_ptr<Weapon_> droppedWeapon = player->dropPrimaryWeapon();
                            if (droppedWeapon == nullptr)
                                continue;
                            droppedWeapons.emplace_back(std::move(droppedWeapon),
                                                        Vec2D(player->getX(), player->getY()));
                        }

                        // pickup del nueva arma
                        player->setPrimaryWeapon(std::move(it->weapon));
                        player->setState(PlayerState::PickingUp);
                        droppedWeapons.erase(it);
                        break;
                    }
                }

                if (bomb.isDropped() &&
                    PhysicsEngine::playerTouchingItem(player->getX(), player->getY(), bomb.getX(),
                                                      bomb.getY())) {
                    if (player->getTeam() == Team::Terrorist) {
                        bomb.pickUp(player->getId());
                        player->setBomb(&bomb);
                    }
                }
                return;
            }
            case GameActionType::Rotate: {
                // std::cout << "recibí rotar" << std::endl;
                player->setAngle(action.gameAction.angle);
                break;
            }
            // case GameActionType::PlantBomb: {
            //     processPlant(action.player_username);
            //     break;
            // }
            case GameActionType::DefuseBomb: {
                processDefuse(action.player_username);
                break;
            }
            default:
                break;
        }
    } else if (phase == GamePhase::Preparation) {
        switch (gameAction.type) {
            case GameActionType::BuyWeapon: {
                if (!Shop::buyPrimaryWeapon(*player, gameAction.weapon, droppedWeapons)) {
                    std::cerr << "Compra de arma fallida. Revise su saldo\n";
                }
                break;
            }
            case GameActionType::BuyAmmo: {
                if (!Shop::buyAmmo(*player, gameAction.ammoType, gameAction.count_ammo)) {
                    std::cerr << "Compra de munición fallida. Revise su saldo\n";
                }
                break;
            }
            default:
                break;
        }
    }
}

void Match::updateState(double elapsedTime) {
    // Eliminamos proyectiles inactivos
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
                                     [](const Projectile& p) { return !p.isActive(); }),
                      projectiles.end());

    // for (auto& player: players) {
    //     if (player.isAlive())
    //         player.setState(PlayerState::Idle);
    // }

    if (roundOver)
        return;
    current_time += elapsedTime;
    roundTimer -= elapsedTime;
    // std::cout << "roundTimer " << roundTimer << std::endl;
    if (phase == GamePhase::Preparation) {
        if (roundTimer <= 0)
            advancePhase();
        return;
    }
    if (phase == GamePhase::Combat && roundTimer <= 0 && !bomb.isPlanted()) {
        roundOver = true;
        roundWinner = Team::CounterTerrorist;
    }

    bomb.update(elapsedTime);
    if (bomb.hasExploded()) {
        roundOver = true;
        roundWinner = Team::Terrorist;
    }


    // Actualizamos los proyectiles
    for (auto& proj: projectiles) {
        if (!proj.isActive())
            continue;

        proj.update(static_cast<float>(elapsedTime));

        if (proj.hasExceededRange()) {
            proj.deactivate();
            continue;
        }

        for (auto& target: players) {
            if (target.getId() == proj.getShooter() || !target.isAlive())
                continue;

            float impactDist;
            bool hitByPrecision = true;
            // const std::unique_ptr<Weapon_> weapon = WeaponFactory::create(proj.getWeaponUsed());
            const std::unique_ptr<Weapon_> rawWeapon = WeaponFactory::create(proj.getWeaponUsed());
            auto* weapon = dynamic_cast<FireWeapon*>(rawWeapon.get());
            if (!weapon)
                continue;
            Player* shooter = getPlayer(proj.getShooter());

            if (PhysicsEngine::shotHitPlayer(proj.getX(), proj.getY(), *shooter, target, *weapon,
                                             impactDist, hitByPrecision)) {
                if (!hitByPrecision) {
                    proj.deactivate();
                    break;
                }
                if (!isFriendlyFire(proj.getShooter(), target.getTeam())) {
                    int damage = weapon->calculateDamage(impactDist);
                    target.takeDamage(damage);
                }

                if (!target.isAlive()) {
                    // actualizo las estadisticas
                    shooter->stats.registerKill();
                    shooter->addMoney(KILL_BONUS);

                    // Dropeo su arma
                    std::unique_ptr<Weapon_> droppedWeapon = target.dropPrimaryWeapon();
                    if (droppedWeapon != nullptr) {
                        droppedWeapons.emplace_back(std::move(droppedWeapon),
                                                    Vec2D(target.getX(), target.getY()));
                    }
                    if (bomb.isCarriedBy(target.getId())) {
                        bomb.drop(target.getX(), target.getY());
                    }
                }
                proj.deactivate();

                break;
            }
        }

        // Detectamos colisión con pared
        if (!map.isWalkable(static_cast<int>(proj.getX()), static_cast<int>(proj.getY()))) {
            proj.deactivate();
        }
    }

    checkRoundEnd();

    if (roundOver || roundTimer <= 0) {
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
        player->setBomb(nullptr);
        player->setEquippedWeapon(TypeWeapon::Primary);
    }
}

void Match::processDefuse(const std::string& playerName) {
    Player* player = getPlayer(playerName);
    if (!player || !player->isAlive() || player->getTeam() != Team::CounterTerrorist)
        return;

    if (bomb.isPlanted() && PhysicsEngine::playerTouchingItem(player->getX(), player->getY(),
                                                              bomb.getX(), bomb.getY())) {
        if (bomb.defuse()) {
            std::cout << "Player " << playerName << " defused the bomb!\n";
            // roundOver = true;
            // roundWinner = Team::CounterTerrorist;
        }
    }
}

void Match::checkRoundEnd() {
    if (roundOver || phase == GamePhase::Preparation || players.size() == 1)
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
            return;
        }
        roundOver = true;
        roundWinner = Team::CounterTerrorist;
    } else if (!antiterroristsLeft) {
        roundOver = true;
        roundWinner = Team::Terrorist;
    }
    if (bomb.getState() == BombState::Defused) {
        roundOver = true;
        roundWinner = Team::CounterTerrorist;
    }
}

bool Match::containsPlayer(const std::string& username) const {
    for (const Player& p: players) {
        if (p.getId() == username) {
            return true;
        }
    }
    return false;
}

LocalPlayerInfo Match::generateLocalPlayerInfo(const std::string& username) const {
    for (const Player& p: players) {
        if (p.getId() == username) {
            return p.generateLocalPlayerInfo();
        }
    }
    throw std::runtime_error("El Player no existe en la partida");
}
// MatchInfo Match::generateMatchInfo() const {
//     MatchInfo mattchInfo(name)
// }
int Match::countPlayers() const { return players.size(); }
const std::string& Match::getIdScenario() const { return id_scenario; }

GameInfo Match::generateGameInfo(const std::string& username) const {

    LocalPlayerInfo localPlayerInfo;
    std::vector<PlayerInfo> playersInfo;
    std::vector<WeaponInfo> weaponsInfo;
    std::vector<BulletInfo> bulletsInfo;

    // std::cout << "hay " << players.size() << " players" << std::endl;
    for (const Player& p: players) {
        if (p.getId() == username) {
            localPlayerInfo = p.generateLocalPlayerInfo();
        } else {
            playersInfo.emplace_back(p.generatePlayerInfo());
        }

        // cargo info del arma equipada. (si tiene la bomba ya se cargó)
        if (p.getEquippedWeapon() != TypeWeapon::Bomb) {
            auto weapon_ptr = p.getEquippedWeaponInstance();
            if (weapon_ptr == nullptr) {
                continue;
            }
            WeaponInfo w(weapon_ptr->generateWeaponInfo(WeaponState::EQUIPPED));
            weaponsInfo.emplace_back(w);
        }

        if (p.getEquippedWeapon() != TypeWeapon::Primary) {
            // cargo info del primary weapon como hidden si no la está usando.
            Weapon_* p_weapon = p.getPrimaryWeapon();
            if (p_weapon != nullptr)
                weaponsInfo.emplace_back(p_weapon->generateWeaponInfo(WeaponState::HIDDEN));
        }
    }

    // cargo bullets
    for (auto& p: projectiles) {
        bulletsInfo.emplace_back(p.getServerId(), p.getWeaponUsed(), p.getX(), p.getY(),
                                 Vec2D(p.getDirX(), p.getDirY()), p.isActive());
    }
    // cargo drops
    for (auto& droppedWeapon: droppedWeapons) {
        weaponsInfo.emplace_back(droppedWeapon.generateWeaponInfo());
    }

    float timeLeft = roundTimer;
    if (bomb.isPlanted()) {
        timeLeft = bomb.getTimer();
    }

    if (phase == GamePhase::EndOfMatch) {
        StatsInfo stats = generateStatsInfo();
        return GameInfo(phase, bomb.generateBombInfo(), timeLeft, localPlayerInfo, playersInfo,
                        bulletsInfo, weaponsInfo, stats);
    }


    return GameInfo(this->phase, bomb.generateBombInfo(), timeLeft, localPlayerInfo, playersInfo,
                    bulletsInfo, weaponsInfo);
}

// void Match::showPlayers() const {
//     std::cout << "Players in match:\n";
//     for (const auto& p: players) {
//         std::cout << " - Player " << p.getId() << " en (" << p.getX() << "," << p.getY() << ") "
//                   << (p.isAlive() ? "[VIVO]" : "[MUERTO]") << "\n";
//     }
// }

void Match::handleKnifeAttack(Player* attacker, const Vec2D& direction) {
    for (auto& target: players) {
        if (target.getId() == attacker->getId() || !target.isAlive())
            continue;

        float impactDistance;
        if (PhysicsEngine::knifeHit(attacker->getX(), attacker->getY(), direction.getX(),
                                    direction.getY(), target, impactDistance) &&

            !isFriendlyFire(attacker->getId(), target.getTeam())) {
            Weapon_* knife = attacker->getEquippedWeaponInstance();
            int damage = knife->calculateDamage(0);
            target.takeDamage(damage);
        }
    }
}

void Match::advancePhase() {
    if (phase == GamePhase::Preparation) {
        // Limpieza entre rondas
        for (auto& p: players) {
            p.setBomb(nullptr);
        }
        bomb.reset();
        projectiles.clear();
        // droppedWeapons.clear(); // esto puede ser mejor mantenerlas

        for (auto& p: players) {
            setPosSpawnPlayer(p);
        }

        // Asignar la bomba a un terrorista vivo
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
            bombCarrier->setBomb(&bomb);
        }

        roundOver = false;
        roundTimer = ROUND_DURATION;
        phase = GamePhase::Combat;
    } else if (phase == GamePhase::Combat) {
        roundsPlayed++;

        // Cambio de lados a la mitad
        if (roundsPlayed == MAX_ROUNDS / 2) {
            for (auto& p: players) {
                Team newTeam =
                        (p.getTeam() == Team::Terrorist) ? Team::CounterTerrorist : Team::Terrorist;
                p.setTeam(newTeam);
            }
        }

        // Desactivamos todos los proyectiles para que se dejen de renderizar
        for (auto& proj: projectiles) {
            if (proj.isActive())
                proj.deactivate();
        }



        // Fin de la partida
        if (roundsPlayed >= MAX_ROUNDS) {
            rankPlayers();
            phase = GamePhase::EndOfMatch;
            return;
        }

        for (auto& p: players) {
            p.revive();
            p.setEquippedWeapon(TypeWeapon::Knife);
        }

        // Pasar a la fase de preparacion
        roundTimer = PREPARATION_TIME;
        phase = GamePhase::Preparation;
        roundOver = false;

        for (auto& p: players) {
            float moneyBonus = BASE_MONEY_BONUS;
            if (p.getTeam() == roundWinner) {
                moneyBonus += WIN_BONUS;
            }
            p.addMoney(moneyBonus);
        }
    }
}

GamePhase Match::getGamePhase() const { return phase; }

void Match::setPosSpawnPlayer(Player& p) {
    Team team = p.getTeam();
    if (team == Team::Terrorist) {
        p.setPostion(map.getPositionTZone());
    } else {
        p.setPostion(map.getPositionCTZone());
    }
}

bool Match::isFriendlyFire(const std::string& shooterId, Team targetTeam) const {
    auto it = std::find_if(players.begin(), players.end(),
                           [&shooterId](const Player& p) { return p.getId() == shooterId; });

    if (it == players.end()) {
        return true;
    }

    const Player& shooter = *it;
    return shooter.getTeam() == targetTeam;
}

void Match::resetStatesOfPlayers() {
    for (auto& player: players) {
        if (player.isAlive())
            player.setState(PlayerState::Idle);
    }
}

void Match::rankPlayers() {
    std::vector<Player*> terrorists;
    std::vector<Player*> counterterrorists;

    for (auto& p: players) {
        if (p.getTeam() == Team::Terrorist)
            terrorists.push_back(&p);
        else
            counterterrorists.push_back(&p);
    }

    // Ordenamos por kill - death ratio
    auto compareByScore = [](Player* a, Player* b) {
        int scoreA = a->stats.kills - a->stats.deaths;
        int scoreB = b->stats.kills - b->stats.deaths;
        return scoreA > scoreB;
    };

    std::sort(terrorists.begin(), terrorists.end(), compareByScore);
    std::sort(counterterrorists.begin(), counterterrorists.end(), compareByScore);
}

StatsInfo Match::generateStatsInfo() const {
    StatsInfo info;
    for (const auto& p: players) {
        const auto& s = p.getStats();
        PlayerStatsInfo psi(p.getId(), s.kills, s.deaths, s.moneyEarned);
        if (p.getTeam() == Team::Terrorist)
            info.terroristStats.push_back(psi);
        else
            info.counterStats.push_back(psi);
    }
    return info;
}
