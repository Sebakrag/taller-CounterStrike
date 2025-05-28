#ifndef POSITIONCOMPONENT_H
#define POSITIONCOMPONENT_H

#include <utility>

#include "client/include/model/utils/Vec2D.h"

class PositionComponent {
private:
    Vec2D position;

public:
    PositionComponent() = default;
    PositionComponent(float x, float y);

    void init(float x, float y);
    void update(float x, float y);

    Vec2D getPosition() const;
};

#endif  // POSITIONCOMPONENT_H
