#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "../../../common/types.h"
#include "../id_generator.h"

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
    uint32_t serverId;

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

    uint32_t getServerId() const;
};

#endif
