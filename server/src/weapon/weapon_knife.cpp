#include "../../include/weapon/weapon_knife.h"

//-------------
// Inicializo las variables estáticas (para poder compilar)
bool WeaponKnife::initialized = false;
int WeaponKnife::DAMAGE = 0;


void WeaponKnife::init(int damage) {
    if (initialized == false) {
        DAMAGE = damage;
        initialized = true;
    }
}
//------------

WeaponKnife::WeaponKnife(): Weapon_(DAMAGE) {}

bool WeaponKnife::canShoot(uint64_t /*currentTimeMs*/) const {
    return true;  // Siempre se puede atacar
}

uint64_t WeaponKnife::getCooldownMs() const {
    return 500;  // 0.5s entre cuchillazos
}

Weapon WeaponKnife::getWeaponType() const { return Weapon::Knife; }

WeaponInfo WeaponKnife::generateWeaponInfo(const WeaponState& state) {
    return {serverId, Weapon::Knife, state, 0, 0, 0};
}

std::vector<Projectile> WeaponKnife::shoot(float /*posX*/, float /*posY*/, float /*dirX*/,
                                           float /*dirY*/, const std::string& /*shooter*/,
                                           uint64_t /*currentTimeMs*/) {
    return {};
}
