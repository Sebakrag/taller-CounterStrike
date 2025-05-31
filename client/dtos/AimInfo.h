#ifndef AIMINFO_H
#define AIMINFO_H

#include "client/include/model/utils/Vec2D.h"

struct AimInfo {
    const Vec2D direction;
    const float angle;

    AimInfo(const Vec2D& dir, const float angle): direction(dir), angle(angle) {}
};

#endif  // AIMINFO_H
