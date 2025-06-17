#ifndef DYNAMICSTENCIL_H
#define DYNAMICSTENCIL_H

#include <memory>

#include <SDL2pp/SDL2pp.hh>

#include "client/dtos/FOVInfo.h"

using SDL2pp::Renderer;
using SDL2pp::Texture;

class DynamicStencil {
private:
    static std::shared_ptr<Texture> stencil;  // TODO: quizas es mas acertado que sea un unique_ptr
    static int screenWidth;                   // in pixels
    static int screenHeight;                  // in pixels
    static float cirRadius;
    static float fovAngle;  // in degrees
    static float fovRadius;

    static void drawCircle(const Renderer& ren, float cx, float cy);
    static void drawFOVTriangle(const Renderer& ren, float cx, float cy);

public:
    DynamicStencil() = delete;

    ///
    /// @param ren Renderer to use.
    /// @param screenW Width of the screen.
    /// @param screenH Height of the screen.
    /// @param circleRadius Radius of the FOV circle that surrounds the player.
    /// @param fovAngleDegrees Angle of the FOV triangle for the player.
    /// @param fovVisibleDistance Radius of the visible distance of the player.
    /// @param stencilAlpha Alpha value for transparency of the not visible area.
    ///
    static void init(Renderer& ren, int screenW, int screenH, float circleRadius,
                     float fovAngleDegrees, uint8_t stencilAlpha, float fovVisibleDistance = 0);

    static std::shared_ptr<Texture> getStencil();
    static FOVInfo getFOVInfo();
};

#endif  // DYNAMICSTENCIL_H
