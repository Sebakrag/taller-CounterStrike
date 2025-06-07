#include "../../include/weapon/weapon_knife.h"

WeaponKnife::WeaponKnife() : Weapon_(20){}

int WeaponKnife::use(uint64_t /*currentTimeMs*/) {
    return damage;
}

bool WeaponKnife::canShoot(uint64_t /*currentTimeMs*/) const {
    return true; //Siempre se puede atacar
}

uint64_t WeaponKnife::getCooldownMs() const {
    return 500; //0.5s entre cuchillazos
}

Weapon WeaponKnife::getWeaponType() const {
    return Weapon::Knife;
}




