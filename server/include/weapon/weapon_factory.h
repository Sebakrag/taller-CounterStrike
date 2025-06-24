#ifndef WEAPON_FACTORY_H
#define WEAPON_FACTORY_H

#include <memory>

#include "../../../common/types.h"

#include "weapon_ak47.h"
#include "weapon_awp.h"
#include "weapon_glock.h"
#include "weapon_knife.h"
#include "weapon_m3.h"

class WeaponFactory {
public:
    static std::unique_ptr<Weapon_> create(Weapon type);
};

#endif
