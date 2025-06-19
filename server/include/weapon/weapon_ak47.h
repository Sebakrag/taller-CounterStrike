#ifndef WEAPON_AK47_H
#define WEAPON_AK47_H

#include <string>
#include <vector>

#include "../../../common/types.h"

#include "fire_weapon.h"

class WeaponAk47: public FireWeapon {
public:
    WeaponAk47();

    Weapon getWeaponType() const override;
    WeaponInfo generateWeaponInfo(const WeaponState& state) override;
    std::vector<Projectile> shoot(float posX, float posY, float dirX, float dirY,
                                  const std::string& shooter, uint64_t currentTimeMs) override;

    // CONSTANTES static para inicializar en server.cpp.
private:
    static bool initialized;  // para asegurar que se llame init una unica vez.
    static int DAMAGE;
    static float PRICE;
    static int INITIAL_BULLETS;
    static int RATE_OF_FIRE;

public:
    // Para cargar los valores del config.yaml
    static void init(int damage, float price, int bullets, int rate_of_fire);
};

#endif
