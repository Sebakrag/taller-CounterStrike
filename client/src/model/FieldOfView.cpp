#include "client/include/model/FieldOfView.h"

#include <cmath>

#include "client/include/model/Graphics.h"
#include "client/include/model/utils/DynamicStencil.h"


FieldOfView::FieldOfView():
        stencil(DynamicStencil::getStencil()),
        info(DynamicStencil::getFOVInfo()),
        currentPlayerAimAngle(0) {}

void FieldOfView::render(Graphics& graphics, const Vec2D& playerPos, const float rotAngle) {
    currentPlayerPos = playerPos;
    currentPlayerAimAngle = rotAngle;

    Rect destRect(-info.screenWidth / 2, -info.screenHeight / 2, info.screenWidth * 2,
                  info.screenHeight * 2);
    Point rotCenter(info.screenWidth, info.screenHeight);
    graphics.draw(*stencil, SDL2pp::NullOpt, destRect, rotAngle, rotCenter);
}

bool FieldOfView::isInFOV(const Vec2D& enttWorldPos) const {
    const Vec2D dif = enttWorldPos - currentPlayerPos;
    std::cout << "[Visibility radius]: " << info.visibilityRadius << std::endl;
    if (dif.calculateNormSquared() > info.visibilityRadius * info.visibilityRadius)
        return false;

    // Calculate the angle between the player and the entity
    float angleToEntity = std::atan2(dif.getY(), dif.getX());

    float deltaAngle = std::abs(WrapAngle(angleToEntity - currentPlayerAimAngle));
    float halfFOVRad = (info.fovAngle / 2.0f) * M_PI / 180.0f;

    return deltaAngle <= halfFOVRad;
}

float FieldOfView::WrapAngle(float angle) const {
    while (angle < -M_PI) angle += 2.0f * M_PI;
    while (angle > M_PI) angle -= 2.0f * M_PI;
    return angle;
}
