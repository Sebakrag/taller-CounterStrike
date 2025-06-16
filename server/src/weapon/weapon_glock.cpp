#include "../../include/weapon/weapon_glock.h"

WeaponGlock::WeaponGlock(): FireWeapon(45, 0, 40, 700) {}

Weapon WeaponGlock::getWeaponType() const { return Weapon::Glock; }

WeaponInfo WeaponGlock::generateWeaponInfo(const WeaponState& state) {
    return {serverId, Weapon::Glock, state, bullets, 0, 0};
}

std::vector<Projectile> WeaponGlock::shoot(float posX, float posY, float dirX, float dirY,
                                           const std::string& shooter, uint64_t currentTimeMs) {
    if (!canShoot(currentTimeMs))
        return {};

    lastShotTimeMs = currentTimeMs;
    bullets--;

    Projectile p(posX, posY, dirX, dirY, 500.0f, 1000.0f, shooter, Weapon::Glock);
    return {p};
}
