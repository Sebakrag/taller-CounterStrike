#ifndef FIELDOFVIEW_H
#define FIELDOFVIEW_H

#include <memory>

#include "common/utils/Vec2D.h"
#include "client/dtos/FOVInfo.h"
#include "SDL2pp/Texture.hh"

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

    ///
    /// @brief Wraps any angle to keep it within the range [-π, π].
    ///
    /// @param angle Angle in radians.
    /// @return Angle equivalent in direction to the given, but within the range [-π, π].
    ///
    float WrapAngle(float angle);

public:
    FieldOfView();

    void render(Graphics& graphics, const Vec2D& playerPos, float rotAngle) const;

    bool isInFOV(const Vec2D& enttWorldPos, const Vec2D& playerPos, float playerAimAngle);
};

#endif //FIELDOFVIEW_H
