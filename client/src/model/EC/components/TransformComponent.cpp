#include "client/include/model/EC/components/TransformComponent.h"

TransformComponent::TransformComponent(const float x, const float y, const float rotAngle):
        position(x, y), rotAngle(rotAngle) {}

void TransformComponent::init(const float x, const float y, const float rotAngle) {
    update(x, y, rotAngle);
}

void TransformComponent::update(const float x, const float y, const float rotAngle) {
    position.set(x, y);
    this->rotAngle = rotAngle;
}

Vec2D TransformComponent::getPosition() const { return position; }

float TransformComponent::getRotationAngle() const { return rotAngle; }
