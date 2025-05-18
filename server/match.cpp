#include "match.h"
#include <iostream>

Match::Match() : phase(MatchPhase::WAITING), roundsPlayed(0) {};

void Match::addPlayer(Player&& player) {
    players.emplace_back(std::move(player));
}

Player* Match::getPlayer(const int playerId) {
    for (auto& p : players) {
        if (p.getId() == playerId)
            return &p;
    }
    return nullptr;
}

bool Match::movePlayer(const int playerId, const int dx, const int dy) {
    Player* p = getPlayer(playerId);
    if (!p || !p->isAlive())
        return false;

    const int newX = p->getX() + dx;
    const int newY = p->getY() + dy;

    if (map.isWalkable(newX, newY)) {
        p->setPosition(newX, newY);
        return true;
    }

    return false;
}

void Match::processAction(const int playerId, const Action &action) {
    Player* player = getPlayer(playerId);
    if (!player || !player->isAlive())
        return;

    switch (action.getType()) {
        case ActionType::MOVE: {
            const int dx = action.getParam("dx");
            const int dy = action.getParam("dy");
            movePlayer(playerId, dx, dy);
            break;
        }
        case ActionType::EQUIP: {
            int weaponType = action.getParam("weaponType");
            player->setEquippedWeapon(static_cast<WeaponType>(weaponType));
            break;
        }
        case ActionType::SHOOT: {
            const int targetX = action.getParam("x");
            const int targetY = action.getParam("y");
            const int dmg = player->attack(targetX, targetY);
            std::cout << "Player " << playerId << " shot with " << dmg << " damage\n";
            break;
        }
        case ActionType::PLANT:
            processPlant(playerId);
            break;
        case ActionType::DEFUSE:
            processDefuse(playerId);
            break;
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
            roundWinner = PlayerType::TERRORIST;
        }
    }

    checkRoundEnd();
}


void Match::processPlant(const int playerId) {
    Player* player = getPlayer(playerId);
    if (!player || !player->isAlive() || player->getType() != PlayerType::TERRORIST)
        return;
    if (bombPlanted)
        return;

    int x = player->getX();
    int y = player->getY();
    if (!map.isBombSite(x, y))
        return;

    bombPlanted = true;
    bombPosX = x;
    bombPosY = y;
    bombTimer = TIME_TO_EXPLODE;

    std::cout << "Player " << playerId << " planted the bomb at (" << x << ", " << y << ")\n";
}

void Match::processDefuse(const int playerId) {
    Player* player = getPlayer(playerId);
    if (!player || !player->isAlive() || player->getType() != PlayerType::ANTITERRORIST)
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
    roundWinner = PlayerType::ANTITERRORIST;

    std::cout << "Player " << playerId << " defused the bomb!\n";
}

void Match::checkRoundEnd() {
    if (roundOver)
        return;

    bool terroristsLeft = false;
    bool antiterroristsLeft = false;

    for (const auto& p : players) {
        if (!p.isAlive()) continue;
        if (p.getType() == PlayerType::TERRORIST) terroristsLeft = true;
        if (p.getType() == PlayerType::ANTITERRORIST) antiterroristsLeft = true;
    }

    if (!terroristsLeft) {
        roundOver = true;
        roundWinner = PlayerType::ANTITERRORIST;
        std::cout << "Todos los terroristas murieron. Ganan los antiterroristas. \n";
    } else if (!antiterroristsLeft) {
        roundOver = true;
        roundWinner = PlayerType::TERRORIST;
        std::cout << "Todos los antiterroristas murieron. Ganan los terroristas. \n";
    } else if (roundTimer <= 0 && !bombPlanted) {
        roundOver = true;
        roundWinner = PlayerType::ANTITERRORIST;
        std::cout << "Se acabó el tiempo sin bomba. Ganan los antiterroristas. \n";
    }
}

GameInfo Match::generateGameInfo(const int playerId) const {
    GameInfo info;

    for (const auto& p : players) {
        if (p.getId() == playerId) {
            info.posX = p.getX();
            info.posY = p.getY();
            info.health = p.getHealth();
            info.equippedWeapon = p.getEquippedWeapon();
            info.bullets = p.getEquippedWeapon() == WeaponType::PRIMARY && p.getPrimaryWeapon() ?
                           p.getPrimaryWeapon()->getBullets() :
                           (p.getEquippedWeapon() == WeaponType::SECONDARY && p.getSecondaryWeapon() ?
                            p.getSecondaryWeapon()->getBullets() : 0);
        } else {
            info.otherPlayers.push_back(PlayerInfo{
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

void Match::showPlayers() const {
    std::cout << "Players in match:\n";
    for (const auto& p : players) {
        std::cout << " - Player " << p.getId()
                  << " en (" << p.getX() << "," << p.getY() << ") "
                  << (p.isAlive() ? "[VIVO]" : "[MUERTO]") << "\n";
    }
}