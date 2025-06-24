#ifndef FIELDOFVIEW_H
#define FIELDOFVIEW_H

#include <memory>

#include "../../../common/dtos/FovConfig.h"
#include "../../../common/utils/Vec2D.h"
#include "SDL2pp/Texture.hh"

class Graphics;

class FieldOfView {
private:
    std::shared_ptr<SDL2pp::Texture> stencil;
    FOVConfig info;
    Vec2D currentPlayerPos;
    float currentPlayerAimAngleRad;

public:
    FieldOfView();

    ///
    /// @brief It renders the Field Of View of the player rotate in the given angle, setting
    /// the player position as the rotation axe. At the same time, it updates those values.
    ///
    void render(Graphics& graphics, const Vec2D& playerPos, float rotAngleDeg);

    bool isInFOV(const Vec2D& enttWorldPos) const;
};

#endif  // FIELDOFVIEW_H
