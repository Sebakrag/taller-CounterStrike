#ifndef AIMINFO_H
#define AIMINFO_H

#include "../../common/types.h"
#include "../../common/utils/Vec2D.h"

struct AimInfo {
    const Vec2D direction;
    const float angle;
    Weapon currentWeapon;

    AimInfo(const Vec2D& dir, const float angle, Weapon weapon):
            direction(dir), angle(angle), currentWeapon(weapon) {}
};

#endif  // AIMINFO_H
