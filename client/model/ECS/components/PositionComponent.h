#ifndef POSITIONCOMPONENT_H
#define POSITIONCOMPONENT_H

#include "client/model/utils/Vec2D.h"

struct PositionComponent {
public:
    Vec2D position;

    PositionComponent() = default;
    PositionComponent(const float x, const float y);
};

#endif //POSITIONCOMPONENT_H
