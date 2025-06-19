#include "../../include/weapon/weapon_ak47.h"

//-------------
// Inicializo las variables estáticas (para poder compilar)
bool WeaponAk47::initialized = false;
int WeaponAk47::DAMAGE = 0;
float WeaponAk47::PRICE = 0;
int WeaponAk47::INITIAL_BULLETS = 0;
int WeaponAk47::RATE_OF_FIRE = 0;


void WeaponAk47::init(int damage, float price, int bullets, int rate_of_fire) {
    if (initialized == false) {
        DAMAGE = damage;
        PRICE = price;
        INITIAL_BULLETS = bullets;
        RATE_OF_FIRE = rate_of_fire;
        initialized = true;
    }
}
//------------

WeaponAk47::WeaponAk47(): FireWeapon(DAMAGE, PRICE, INITIAL_BULLETS, RATE_OF_FIRE) {}

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
    for (int i = 0; i < bulletsToShoot; ++i) {
        Projectile p(posX, posY, dirX, dirY, 450.0f, 900.0f, shooter, Weapon::Ak47);
        projectiles.push_back(p);
    }

    return projectiles;
}
