#include "client/include/model/EC/components/TransformComponent.h"

#include "common/utils/AngleUtils.h"

TransformComponent::TransformComponent(const float posX, const float posY, const float rotAngleDeg):
        position(posX, posY), rotAngleDeg(AngleUtils::normalizeDegrees_0_360(rotAngleDeg)) {}

void TransformComponent::init(const float x, const float y, const float rotAngleDeg) {
    update(x, y, rotAngleDeg);
}

void TransformComponent::update(const float x, const float y, const float rotAngleDeg) {
    position.set(x, y);
    this->rotAngleDeg = AngleUtils::normalizeDegrees_0_360(rotAngleDeg);
}

Vec2D TransformComponent::getPosition() const { return position; }

float TransformComponent::getRotationAngleDegrees() const { return rotAngleDeg; }

float TransformComponent::getRotationAngleRadian() const {
    return AngleUtils::degreesToRadians(rotAngleDeg);
}
