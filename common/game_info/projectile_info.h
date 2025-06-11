#ifndef PROJECTILE_INFO_H
#define PROJECTILE_INFO_H

#include <string>
#include "../types.h"

struct ProjectileInfo {
    float x;
    float y;
    float dirX;
    float dirY;
    std::string shooterUsername;
    Weapon weapon;
    bool active;

    ProjectileInfo(float x, float y, float dirX, float dirY,
                   const std::string& shooterUsername, Weapon weapon, bool active)
        : x(x), y(y), dirX(dirX), dirY(dirY), shooterUsername(shooterUsername), weapon(weapon), active(active) {}
};

#endif