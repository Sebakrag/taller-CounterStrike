#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "../../../common/types.h"

class Projectile {
private:
    float posX, posY;
    float dirX, dirY;
    float speed;
    float traveledDistance;
    float maxDistance;
    std::string shooter;
    bool active;
    Weapon weaponUsed;

public:
    Projectile(float posX, float posY, float dirX, float dirY, float speed, float maxDistance, std::string shooter, Weapon weaponUsed);

    void update(float deltaTime);
    bool hasExceededRange() const;
    bool isActive() const;
    void deactivate();

    float getX() const;
    float getY() const;
    std::string getShooter() const;
    float getDirX() const;
    float getDirY() const;
    float getMaxDistance() const;
    Weapon getWeaponUsed() const;
};

#endif
