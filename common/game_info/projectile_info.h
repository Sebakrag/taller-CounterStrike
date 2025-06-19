#ifndef PROJECTILE_INFO_H
#define PROJECTILE_INFO_H

#include <string>
#include <iostream>

#include "../types.h"
#include "../protocol.h"

struct ProjectileInfo {
    float x;
    float y;
    float dirX;
    float dirY;
    std::string shooterUsername;
    Weapon weapon;
    bool active;

    ProjectileInfo();
    ProjectileInfo(float x, float y, float dirX, float dirY,
                   const std::string& shooterUsername, Weapon weapon, bool active);

    explicit ProjectileInfo(const std::vector<uint8_t>& bytes);
    std::vector<uint8_t> toBytes() const;

    void print() const;
};

#endif