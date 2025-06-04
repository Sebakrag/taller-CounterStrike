#ifndef EQUIPPEDWEAPONCOMPONENT_H
#define EQUIPPEDWEAPONCOMPONENT_H

#include "client/include/model/EC/Entity.h"

class EquippedWeaponComponent {
private:
    Entity weaponID;

public:
    EquippedWeaponComponent() = default;

    void setID(const Entity newID) {
        weaponID = newID;
    }

    Entity getID() const { return weaponID; }
};

#endif //EQUIPPEDWEAPONCOMPONENT_H
