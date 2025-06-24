#include "../../include/weapon/weapon_knife.h"

#include <random>

//-------------
// Inicializo las variables estáticas (para poder compilar)
bool WeaponKnife::initialized = false;
int WeaponKnife::DAMAGE = 0;
int WeaponKnife::MAX_DAMAGE = 0;


void WeaponKnife::init(int damage, int max_damage) {
    if (initialized == false) {
        DAMAGE = damage;
        MAX_DAMAGE = max_damage;
        initialized = true;
    }
}
//------------

WeaponKnife::WeaponKnife(): Weapon_(DAMAGE, MAX_DAMAGE) {}

bool WeaponKnife::canShoot(double /*currentTime*/) const {
    return true;  // Siempre se puede atacar
}

double WeaponKnife::getCooldownS() const {
    return 0.5;  // 0.5s entre cuchillazos
}

Weapon WeaponKnife::getWeaponType() const { return Weapon::Knife; }

WeaponInfo WeaponKnife::generateWeaponInfo(const WeaponState& state) {
    return {serverId, Weapon::Knife, state, 0, 0, 0};
}

std::vector<Projectile> WeaponKnife::shoot(float /*posX*/, float /*posY*/, float /*dirX*/,
                                           float /*dirY*/, const std::string& /*shooter*/,
                                           double /*currentTime*/) {
    return {};
}

float WeaponKnife::getMaxDamage() const { return maxDamage; }

int WeaponKnife::calculateDamage(float /*distance*/) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(damage, maxDamage);
    return dist(gen);
}
