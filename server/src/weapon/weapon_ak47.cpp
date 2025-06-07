#include "../../include/weapon/weapon_ak47.h"

WeaponAk47::WeaponAk47(): FireWeapon(35, 2700, 30, 600) {}

Weapon WeaponAk47::getWeaponType() const {
    return Weapon::Ak47;
}

