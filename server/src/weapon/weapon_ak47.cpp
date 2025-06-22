#include "../../include/weapon/weapon_ak47.h"

#include <complex>

#include "yaml-cpp/emittermanip.h"

//-------------
// Inicializo las variables estáticas (para poder compilar)
bool WeaponAk47::initialized = false;
int WeaponAk47::DAMAGE = 0;
float WeaponAk47::PRICE = 0;
int WeaponAk47::INITIAL_BULLETS = 0;
int WeaponAk47::RATE_OF_FIRE = 0;
float WeaponAk47::PRECISION = 0;
float WeaponAk47::MAX_RANGE = 0;
int WeaponAk47::MAX_DAMAGE = 0;


void WeaponAk47::init(int damage, float price, int bullets, int rate_of_fire, float precision, float max_range, int max_damage) {
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

WeaponAk47::WeaponAk47(): FireWeapon(DAMAGE, PRICE, INITIAL_BULLETS, RATE_OF_FIRE, PRECISION, MAX_RANGE, MAX_DAMAGE) {}


Weapon WeaponAk47::getWeaponType() const { return Weapon::Ak47; }

WeaponInfo WeaponAk47::generateWeaponInfo(const WeaponState& state) {
    return {serverId, Weapon::Ak47, state, bullets, 0, 0};
}


std::vector<Projectile> WeaponAk47::shoot(float posX, float posY, float dirX, float dirY,
                                          const std::string& shooter, double currentTime) {
    if (!canShoot(currentTime))
        return {};

    lastShotTimeS = currentTime;

    int bulletsToShoot = std::min(3, bullets);
    bullets -= bulletsToShoot;

    std::vector<Projectile> projectiles;

    // Normalizamos el vector perpendicular
    float length = std::sqrt(dirX * dirX + dirY * dirY);
    float normDirX = dirX;
    float normDirY = dirY;
    if (length != 0) {
        normDirX /= length;
        normDirY /= length;
    }

    // Offset entre proyectiles de la rafaga
    float offset = 12.0f;

    for (int i = 0; i < bulletsToShoot; ++i) {
        float offsetFactor = i * offset;
        float spawnX = posX - normDirX * offsetFactor;
        float spawnY = posY - normDirY * offsetFactor;

        // Projectile p(posX, posY, dirX, dirY, 450.0f, 900.0f, shooter, Weapon::Ak47);
        Projectile p(spawnX, spawnY, dirX, dirY, 450.0f, 900.0f, shooter, Weapon::Ak47);
        projectiles.push_back(p);
    }

    return projectiles;
}
