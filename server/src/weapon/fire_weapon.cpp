#include "../../include/weapon/fire_weapon.h"

#include <iostream>

FireWeapon::FireWeapon(int damage, float price, int bullets, int rateOfFire):
        Weapon_(damage), price(price), bullets(bullets), rateOfFire(rateOfFire), lastShotTimeS(0) {}


bool FireWeapon::canShoot(double currentTime) const {
    if (bullets > 0)
        std::cout << "balas ok" << std::endl;
    std::cout << "currentTime:" << currentTime << std::endl;
    std::cout << "lastShotTimeS:" << lastShotTimeS << std::endl;
    std::cout << "getCooldownS:" << getCooldownS() << std::endl;
    return (bullets > 0) && ((currentTime - lastShotTimeS) >= getCooldownS());
}

double FireWeapon::getCooldownS() const {
    return 60.0f / rateOfFire;  // Balas por minuto a delay entre balas
}

void FireWeapon::addBullets(int amount) { bullets += amount; }

int FireWeapon::getBullets() const { return bullets; }
