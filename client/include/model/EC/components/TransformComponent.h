#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "../../../../../common/utils/Vec2D.h"

class TransformComponent {
private:
    Vec2D position;
    float rotAngleDeg;  // rotation angle in degrees.

public:
    TransformComponent() = default;
    TransformComponent(float posX, float posY, float rotAngleDeg);

    void init(float x, float y, float rotAngleDeg);
    void update(float x, float y, float rotAngleDeg);

    Vec2D getPosition() const;
    float getRotationAngleDegrees() const;
    float getRotationAngleRadian() const;
};

#endif  // TRANSFORMCOMPONENT_H
