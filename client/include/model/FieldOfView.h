#ifndef FIELDOFVIEW_H
#define FIELDOFVIEW_H

#include <memory>

#include "SDL2pp/Texture.hh"
#include "client/dtos/FOVInfo.h"
#include "common/utils/Vec2D.h"

class Graphics;

///
/// cono de visión, de ángulo configurable, donde el jugador puede ver
/// Se creará una textura de forma dinámica (dibujando un
/// triangulo de angulo configurable de color blanco sobre un fondo negro)
///
class FieldOfView {
private:
    std::shared_ptr<SDL2pp::Texture> stencil;
    FOVInfo info;
    Vec2D currentPlayerPos;
    float currentPlayerAimAngle;

    ///
    /// @brief Wraps any angle to keep it within the range [-π, π].
    ///
    /// @param angle Angle in radians.
    /// @return Angle equivalent in direction to the given, but within the range [-π, π].
    ///
    float WrapAngle(float angle) const;

public:
    FieldOfView();

    ///
    /// @brief It renders the Field Of View of the player rotate in the given angle, setting
    /// the player position as the rotation axe. At the same time, it updates those values.
    ///
    void render(Graphics& graphics, const Vec2D& playerPos, float rotAngle);

    bool isInFOV(const Vec2D& enttWorldPos) const;
};

#endif  // FIELDOFVIEW_H
