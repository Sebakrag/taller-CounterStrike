#include "../../include/weapon/weapon_awp.h"

//-------------
// Inicializo las variables estáticas (para poder compilar)
bool WeaponAwp::initialized = false;
int WeaponAwp::DAMAGE = 0;
float WeaponAwp::PRICE = 0;
int WeaponAwp::INITIAL_BULLETS = 0;
int WeaponAwp::RATE_OF_FIRE = 0;
float WeaponAwp::PRECISION = 0;
float WeaponAwp::MAX_RANGE = 0;
int WeaponAwp::MAX_DAMAGE = 0;


void WeaponAwp::init(int damage, float price, int bullets, int rate_of_fire, float precision, float max_range, int max_damage) {
    if (initialized == false) {
        DAMAGE = damage;
        PRICE = price;
        INITIAL_BULLETS = bullets;
        RATE_OF_FIRE = rate_of_fire;
        PRECISION = precision;
        MAX_RANGE = max_range;
        MAX_DAMAGE = max_damage;
        initialized = true;
    }
}
//------------

WeaponAwp::WeaponAwp(): FireWeapon(DAMAGE, PRICE, INITIAL_BULLETS, RATE_OF_FIRE, PRECISION, MAX_RANGE, MAX_DAMAGE) {}

Weapon WeaponAwp::getWeaponType() const { return Weapon::Awp; }

WeaponInfo WeaponAwp::generateWeaponInfo(const WeaponState& state) {
    return {serverId, Weapon::Awp, state, bullets, 0, 0};
}

std::vector<Projectile> WeaponAwp::shoot(float posX, float posY, float dirX, float dirY,
                                         const std::string& shooter, double currentTime) {
    if (!canShoot(currentTime))
        return {};

    lastShotTimeS = currentTime;
    bullets--;

    Projectile p(posX, posY, dirX, dirY, 600.0f, 2000.0f, shooter, Weapon::Awp);
    return {p};
}
