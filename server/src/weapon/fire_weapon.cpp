#include "../../include/weapon/fire_weapon.h"

FireWeapon::FireWeapon(int damage, float price, int bullets, int rateOfFire):
        Weapon_(damage), price(price), bullets(bullets), rateOfFire(rateOfFire), lastShotTimeMs(0) {}

int FireWeapon::use(uint64_t currentTimeMs) {
    if (!canShoot(currentTimeMs))
        return -1;

    bullets--;
    lastShotTimeMs = currentTimeMs;
    return damage;
}

bool FireWeapon::canShoot(uint64_t currentTimeMs) const {
    return (bullets > 0) && ((currentTimeMs - lastShotTimeMs) >= getCooldownMs());
}

uint64_t FireWeapon::getCooldownMs() const {
    return static_cast<uint64_t>(60000.0f / rateOfFire); //Balas por minuto a delay entre balas
}

void FireWeapon::addBullets(int amount) { bullets += amount; }

int FireWeapon::getBullets() const { return bullets; }
