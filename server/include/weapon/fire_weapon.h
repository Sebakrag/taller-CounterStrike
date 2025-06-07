#ifndef FIRE_WEAPON_H
#define FIRE_WEAPON_H

#include "weapon.h"

class FireWeapon: public Weapon_ {
protected:
    float price;
    int bullets;
    int rateOfFire;
    uint64_t lastShotTimeMs;

public:
    FireWeapon(int damage, float price, int bullets, int rateOfFire);
    ~FireWeapon() override = default;

    int use(uint64_t currentTimeMs) override;
    bool canShoot(uint64_t currentTimeMs) const override;
    uint64_t getCooldownMs() const override;

    void addBullets(int amount);
    int getBullets() const override;
};

#endif
