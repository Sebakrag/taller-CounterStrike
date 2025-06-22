#include "../../include/weapon/weapon_glock.h"

//-------------
// Inicializo las variables estáticas (para poder compilar)
bool WeaponGlock::initialized = false;
int WeaponGlock::DAMAGE = 0;
int WeaponGlock::INITIAL_BULLETS = 0;
int WeaponGlock::RATE_OF_FIRE = 0;
float WeaponGlock::PRECISION = 0;
float WeaponGlock::MAX_RANGE = 0;
int WeaponGlock::MAX_DAMAGE = 0;


void WeaponGlock::init(int damage, int bullets, int rate_of_fire, float precision, float max_range, int max_damage) {
    if (initialized == false) {
        DAMAGE = damage;
        INITIAL_BULLETS = bullets;
        RATE_OF_FIRE = rate_of_fire;
        PRECISION = precision;
        MAX_RANGE = max_range;
        MAX_DAMAGE = max_damage;
        initialized = true;
    }
}
//------------

WeaponGlock::WeaponGlock(): FireWeapon(DAMAGE, 0, INITIAL_BULLETS, RATE_OF_FIRE, PRECISION, MAX_RANGE, MAX_DAMAGE) {}

Weapon WeaponGlock::getWeaponType() const { return Weapon::Glock; }

WeaponInfo WeaponGlock::generateWeaponInfo(const WeaponState& state) {
    return {serverId, Weapon::Glock, state, bullets, 0, 0};
}

std::vector<Projectile> WeaponGlock::shoot(float posX, float posY, float dirX, float dirY,
                                           const std::string& shooter, double currentTime) {
    if (!canShoot(currentTime))
        return {};

    lastShotTimeS = currentTime;
    bullets--;

    Projectile p(posX, posY, dirX, dirY, 500.0f, 1000.0f, shooter, Weapon::Glock);
    return {p};
}
