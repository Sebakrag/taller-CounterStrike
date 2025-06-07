#include "../../include/weapon/weapon_glock.h"

WeaponGlock::WeaponGlock(): FireWeapon(45, 0, 40, 700) {}

Weapon WeaponGlock::getWeaponType() const {
    return Weapon::Glock;
}