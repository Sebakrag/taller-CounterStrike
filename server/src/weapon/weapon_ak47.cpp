#include "../../include/weapon/weapon_ak47.h"

WeaponAk47::WeaponAk47(): FireWeapon(35, 2700, 30, 600) {}

Weapon WeaponAk47::getWeaponType() const {
    return Weapon::Ak47;
}

std::vector<Projectile> WeaponAk47::shoot(float posX, float posY, float dirX, float dirY, const std::string &shooter, uint64_t currentTimeMs) {
    if (!canShoot(currentTimeMs)) return {};

    lastShotTimeMs = currentTimeMs;

    int bulletsToShoot = std::min(3, bullets);
    bullets -= bulletsToShoot;

    std::vector<Projectile> projectiles;
    for (int i = 0; i < bulletsToShoot; ++i) {
        Projectile p(posX, posY, dirX, dirY, 450.0f, 900.0f, shooter, Weapon::Ak47);
        projectiles.push_back(p);
    }

    return projectiles;
}

