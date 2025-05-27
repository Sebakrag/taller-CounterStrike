#ifndef POSITIONCOMPONENT_H
#define POSITIONCOMPONENT_H

#include <utility>

#include "client/include/model/utils/Vec2D.h"

class PositionComponent {
private:
    Vec2D position;

public:
    PositionComponent() = default;
    PositionComponent(const float x, const float y);

    void init(const float x, const float y);
    void move(const Vec2D& dir);

    Vec2D getPosition() const;
};

#endif  // POSITIONCOMPONENT_H
