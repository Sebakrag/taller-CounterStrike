#ifndef FIRE_WEAPON_H
#define FIRE_WEAPON_H

#include "weapon.h"

class FireWeapon: public Weapon_ {
protected:
    float price;
    int bullets;
    int rateOfFire;

public:
    FireWeapon(const int damage, const float price, const int bullets, const int rateOfFire);
    ~FireWeapon() override = default;

    int use() override;
    void addBullets(const int amount);
    int getBullets() const;
};

#endif
