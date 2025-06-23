#include "../../include/weapon/weapon_m3.h"

#include <cmath>

//-------------
// Inicializo las variables estáticas (para poder compilar)
bool WeaponM3::initialized = false;
int WeaponM3::DAMAGE = 0;
float WeaponM3::PRICE = 0;
int WeaponM3::INITIAL_BULLETS = 0;
int WeaponM3::RATE_OF_FIRE = 0;
float WeaponM3::PRECISION = 0;
float WeaponM3::MAX_RANGE = 0;
int WeaponM3::MAX_DAMAGE = 0;


void WeaponM3::init(int damage, float price, int bullets, int rate_of_fire, float precision, float max_range, int max_damage) {
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

WeaponM3::WeaponM3(): FireWeapon(DAMAGE, PRICE, INITIAL_BULLETS, RATE_OF_FIRE, PRECISION, MAX_RANGE, MAX_DAMAGE) {}

Weapon WeaponM3::getWeaponType() const { return Weapon::M3; }

WeaponInfo WeaponM3::generateWeaponInfo(const WeaponState& state) {
    return {serverId, Weapon::M3, state, bullets, 0, 0};
}

std::vector<Projectile> WeaponM3::shoot(float posX, float posY, float dirX, float dirY,
                                        const std::string& shooter, double currentTime) {
    if (!canShoot(currentTime))
        return {};

    lastShotTimeS = currentTime;
    bullets--;

    std::vector<Projectile> projectiles;
    const int numPellets = 5;
    const float spreadAngle = 0.15f;

    for (int i = 0; i < numPellets; ++i) {
        float angleOffset = (i - numPellets / 2) * spreadAngle;
        float newDirX = dirX * std::cos(angleOffset) - dirY * std::sin(angleOffset);
        float newDirY = dirX * std::sin(angleOffset) + dirY * std::cos(angleOffset);
        Projectile p(posX, posY, newDirX, newDirY, 400.0f, maxRange, shooter, Weapon::M3);
        projectiles.push_back(p);
    }

    return projectiles;
}
