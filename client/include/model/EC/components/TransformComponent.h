#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <utility>

#include "client/include/model/utils/Vec2D.h"

class TransformComponent {
private:
    Vec2D position;
    float rotAngle;  // rotation angle in degrees.

public:
    TransformComponent() = default;
    TransformComponent(float posX, float posY, float rotAngle);

    void init(float x, float y, float rotAngle);
    void update(float x, float y, float rotAngle);

    Vec2D getPosition() const;
    float getRotationAngle() const;
};

#endif  // TRANSFORMCOMPONENT_H
