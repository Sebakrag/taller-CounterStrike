#include "../../include/weapon/fire_weapon.h"

FireWeapon::FireWeapon(const int damage, const float price, const int bullets, const int rateOfFire)
    : Weapon(damage), price(price), bullets(bullets), rateOfFire(rateOfFire) {}

int FireWeapon::use() {
    if (bullets <= 0)
        return -1;

    bullets--;
    return damage;
}

void FireWeapon::addBullets(const int amount) {
    bullets += amount;
}

int FireWeapon::getBullets() const {
    return bullets;
}

