#ifndef PLAYER_H
#define PLAYER_H

#include <string>

#include <memory>
#include "../../common/types.h"
#include "weapon/weapon.h"
#include "weapon/weapon_factory.h"

class Player {
private:
    std::string name;  // id
    Team team;
    float posX, posY;
    int health;
    PlayerState state;
    float speed = 80.0f;

    std::unique_ptr<Weapon_> knife;
    std::unique_ptr<Weapon_> primaryWeapon;
    std::unique_ptr<Weapon_> secondaryWeapon;
    TypeWeapon equippedWeapon;

    float money;
    int kills;

public:
    explicit Player(const std::string& name, const Team playerTeam);

    void setPrimaryWeapon(std::unique_ptr<Weapon_> weapon);
    void setEquippedWeapon(TypeWeapon type);
    int attack(float targetX, float targetY, uint64_t currentTimeMs);

    float getX() const;
    void setX(const float x);
    float getY() const;
    void setY(const float y);
    std::string getId() const;
    Team getTeam() const;
    int getHealth() const;
    float getMoney() const;
    float getSpeed() const;
    TypeWeapon getEquippedWeapon() const;
    Weapon getSpecificEquippedWeapon() const;
    Weapon_* getPrimaryWeapon() const;
    bool isAlive() const;
    bool canShoot(uint64_t currentTimeMs) const;
    void takeDamage(int dmg);
    int shoot(uint64_t currentTimeMs);
};


#endif
