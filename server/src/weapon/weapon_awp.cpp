#include "../../include/weapon/weapon_awp.h"

WeaponAwp::WeaponAwp(): FireWeapon(100, 3500, 10, 20) {}

Weapon WeaponAwp::getWeaponType() const {
    return Weapon::Awp;
}

std::vector<Projectile> WeaponAwp::shoot(float posX, float posY, float dirX, float dirY, const std::string &shooter, uint64_t currentTimeMs) {
    if (!canShoot(currentTimeMs)) return {};

    lastShotTimeMs = currentTimeMs;
    bullets--;

    Projectile p(posX, posY, dirX, dirY, 600.0f, 2000.0f, shooter, Weapon::Awp);
    return {p};
}
