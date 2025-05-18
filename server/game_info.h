#ifndef GAME_INFO_H
#define GAME_INFO_H

#include <vector>
#include <string>
#include "enums/player_type.h"
#include "enums/weapon_type.h"

struct PlayerInfo {
    int id;
    int x, y;
    bool isAlive;
    PlayerType type;
};

class GameInfo {
public:
    int posX, posY;
    int health;
    WeaponType equippedWeapon;
    int bullets;

    bool bombPlanted;
    int bombX, bombY;
    double timeLeft;

    std::vector<PlayerInfo> otherPlayers;

    std::string toDebugString() const;
};

#endif
