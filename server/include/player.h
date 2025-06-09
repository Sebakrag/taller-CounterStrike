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
    float posX, posY;
    int health;
    PlayerState state;
    float speed = 80.0f;
    float angle = 0;

    WeaponKnife knife;
    FireWeapon* primaryWeapon;
    FireWeapon* secondaryWeapon;
    TypeWeapon equippedWeapon;

    float money;
    int kills;

public:
    explicit Player(const std::string& name, const Team playerTeam);

    void receiveDamage(const int dmg);
    void setPrimaryWeapon(FireWeapon* weapon);
    void setEquippedWeapon(TypeWeapon type);
    int attack(float targetX, float targetY);

    float getX() const;
    void setX(const float x);
    float getY() const;
    void setY(const float y);
    float getAngle() const;
    void setAngle(float angle);
    std::string getId() const;
    Team getTeam() const;
    int getHealth() const;
    float getMoney() const;
    float getSpeed() const;
    TypeWeapon getEquippedWeapon() const;
    FireWeapon* getPrimaryWeapon() const;
    FireWeapon* getSecondaryWeapon() const;
    bool isAlive() const;
};


#endif
