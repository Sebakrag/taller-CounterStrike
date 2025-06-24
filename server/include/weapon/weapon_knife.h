#ifndef WEAPON_KNIFE_H
#define WEAPON_KNIFE_H

#include <string>
#include <vector>

#include "../../../server/include/weapon/weapon.h"

class WeaponKnife: public Weapon_ {
public:
    WeaponKnife();
    ~WeaponKnife() override = default;

    bool canShoot(double currentTime) const override;
    double getCooldownS() const override;
    Weapon getWeaponType() const override;
    WeaponInfo generateWeaponInfo(const WeaponState& state) override;
    std::vector<Projectile> shoot(float posX, float posY, float dirX, float dirY,
                                  const std::string& shooter, double currentTime) override;
    float getMaxDamage() const override;
    int calculateDamage(float distance) const override;


    // CONSTANTES static para inicializar en server.cpp.
private:
    static bool initialized;  // para asegurar que se llame init una unica vez.
    static int DAMAGE;
    static int MAX_DAMAGE;

public:
    // Para cargar los valores del config.yaml
    static void init(int damage, int max_damage);
};

#endif
