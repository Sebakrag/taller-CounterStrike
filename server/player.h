#ifndef PLAYER_H
#define PLAYER_H

#include "weapon_knife.h"
#include "weapon/fire_weapon.h"
#include "enums/weapon_type.h"
#include "enums/player_type.h"
#include "enums/player_state.h"

class Player {
private:
    int id;
    PlayerType type;
    int posX, posY;
    int health;
    PlayerState state;

    WeaponKnife knife;
    FireWeapon* primaryWeapon;
    FireWeapon* secondaryWeapon;
    WeaponType equippedWeapon;

    float money;
    int kills;

public:
    explicit Player(const int id, const PlayerType type);

    void setPosition(const int x, const int y);
    void receiveDamage(const int dmg);
    void setPrimaryWeapon(FireWeapon* weapon);
    void setEquippedWeapon(WeaponType type);
    int attack(/*int targetX, int targetY*/);

    int getX() const;
    int getY() const;
    bool isAlive() const;
};


#endif

