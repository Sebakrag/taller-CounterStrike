#ifndef FIRE_WEAPON_H
#define FIRE_WEAPON_H

#include <string>
#include <vector>

#include "weapon.h"

class FireWeapon: public Weapon_ {
protected:
    float price;
    int bullets;
    int rateOfFire;
    double lastShotTimeS;
    float basePrecision;
    float maxRange;

public:
    FireWeapon(int damage, float price, int bullets, int rateOfFire, float basePrecision, float maxRange);
    ~FireWeapon() override = default;

    bool canShoot(double currentTime) const override;
    double getCooldownS() const override;

    void addBullets(int amount);
    int getBullets() const override;

    float getBasePrecision() const;
    float getMaxRange() const;
};

#endif
