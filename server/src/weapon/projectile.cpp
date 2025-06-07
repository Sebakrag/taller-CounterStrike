#include "server/include/weapon/projectile.h"
#include <cmath>

Projectile::Projectile(float posX, float posY, float dirX, float dirY, float speed, float maxDistance, std::string shooter, Weapon weaponUsed) :
    posX(posX),
    posY(posY),
    dirX(dirX),
    dirY(dirY),
    speed(speed),
    traveledDistance(0),
    maxDistance(maxDistance),
    shooter(shooter),
    active(true),
    weaponUsed(weaponUsed) {}

void Projectile::update(float deltaTime) {
    if (!active)
        return;

    float dx = dirX * speed * deltaTime;
    float dy = dirY * speed * deltaTime;

    posX += dx;
    posY += dy;

    traveledDistance += std::sqrt(dx * dx + dy * dy);

    if (traveledDistance >= maxDistance) {
        active = false;
    }
}

bool Projectile::hasExceededRange() const { return traveledDistance >= maxDistance; }

bool Projectile::isActive() const { return active; }

void Projectile::deactivate() { active = false; }

float Projectile::getX() const { return posX; }
float Projectile::getY() const { return posY; }
std::string Projectile::getShooter() const { return shooter; }
float Projectile::getDirX() const { return dirX; }
float Projectile::getDirY() const { return dirY; }
float Projectile::getMaxDistance() const { return maxDistance; }
Weapon Projectile::getWeaponUsed() const { return weaponUsed; }




