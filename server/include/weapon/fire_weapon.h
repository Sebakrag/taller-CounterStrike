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

public:
    FireWeapon(int damage, float price, int bullets, int rateOfFire);
    ~FireWeapon() override = default;

    bool canShoot(double currentTime) const override;
    double getCooldownS() const override;

    void addBullets(int amount);
    int getBullets() const override;
};

#endif
