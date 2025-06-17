#include "client/include/model/FieldOfView.h"

#include <cmath>

#include "client/include/model/Graphics.h"
#include "client/include/model/utils/DynamicStencil.h"
#include "common/utils/AngleUtils.h"


FieldOfView::FieldOfView():
        stencil(DynamicStencil::getStencil()),
        info(DynamicStencil::getFOVInfo()),
        currentPlayerAimAngleRad(0) {}

void FieldOfView::render(Graphics& graphics, const Vec2D& playerPos, const float rotAngleDeg) {
    currentPlayerPos = playerPos;
    currentPlayerAimAngleRad = AngleUtils::degreesToRadians(rotAngleDeg);

    Rect destRect(-info.screenWidth / 2, -info.screenHeight / 2, info.screenWidth * 2,
                  info.screenHeight * 2);
    Point rotCenter(info.screenWidth, info.screenHeight);
    graphics.draw(*stencil, SDL2pp::NullOpt, destRect, rotAngleDeg, rotCenter);
}

bool FieldOfView::isInFOV(const Vec2D& enttWorldPos) const {
    // Vector desde el player hacia la entidad
    const Vec2D toEntity = enttWorldPos - currentPlayerPos;

    // Chequeo por radio de visibilidad
    const float distSquared = toEntity.calculateNormSquared();
    const float maxDistSquared = info.visibilityRadius * info.visibilityRadius;
    if (distSquared > maxDistSquared)
        return false;

    // Ángulo hacia entre la entidad y el player
    const float angleToEntity = toEntity.calculateAngleRadian();

    // Diferencia angular normalizada a [-π, π]
    float deltaAngle = AngleUtils::normalizeRadians_PI(angleToEntity - currentPlayerAimAngleRad);

    // Comparación contra el ángulo del FOV
    const float halfFOVRad = AngleUtils::degreesToRadians(info.fovAngle * 0.5f);

    return std::fabs(deltaAngle) <= halfFOVRad;
}
