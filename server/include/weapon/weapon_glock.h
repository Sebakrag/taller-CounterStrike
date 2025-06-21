#ifndef WEAPON_GLOCK_H
#define WEAPON_GLOCK_H

#include <string>
#include <vector>

#include "../../../common/types.h"

#include "fire_weapon.h"

class WeaponGlock: public FireWeapon {
public:
    WeaponGlock();

    Weapon getWeaponType() const override;
    WeaponInfo generateWeaponInfo(const WeaponState& state) override;
    std::vector<Projectile> shoot(float posX, float posY, float dirX, float dirY,
                                  const std::string& shooter, double currentTime) override;

    // CONSTANTES static para inicializar en server.cpp.
private:
    static bool initialized;  // para asegurar que se llame init una unica vez.
    static int DAMAGE;
    static int INITIAL_BULLETS;
    static int RATE_OF_FIRE;
    static float PRECISION;
    static float MAX_RANGE;

public:
    // Para cargar los valores del config.yaml
    static void init(int damage, int bullets, int rate_of_fire, float precision, float max_range);
};

#endif
