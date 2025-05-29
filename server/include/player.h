#ifndef PLAYER_H
#define PLAYER_H

#include <string>

#include "../../common/types.h"
#include "weapon/fire_weapon.h"
#include "weapon/weapon_knife.h"

class Player {
private:
    std::string name;  // id
    Team team;
    int posX, posY;
    int health;
    PlayerState state;

    WeaponKnife knife;
    FireWeapon* primaryWeapon;
    FireWeapon* secondaryWeapon;
    TypeWeapon equippedWeapon;

    float money;
    int kills;

public:
    explicit Player(const std::string& name, const Team playerTeam);

    void setPosition(const int x, const int y);
    void receiveDamage(const int dmg);
    void setPrimaryWeapon(FireWeapon* weapon);
    void setEquippedWeapon(TypeWeapon type);
    int attack(int targetX, int targetY);

    int getX() const;
    int getY() const;
    std::string getId() const;
    Team getTeam() const;
    int getHealth() const;
    TypeWeapon getEquippedWeapon() const;
    FireWeapon* getPrimaryWeapon() const;
    FireWeapon* getSecondaryWeapon() const;
    bool isAlive() const;
};


#endif
