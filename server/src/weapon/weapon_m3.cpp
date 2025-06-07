#include "../../include/weapon/weapon_m3.h"

WeaponM3::WeaponM3(): FireWeapon(70, 3000, 20, 500) {}

Weapon WeaponM3::getWeaponType() const {
    return Weapon::M3;
}