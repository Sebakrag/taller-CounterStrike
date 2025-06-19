#ifndef DYNAMICSTENCIL_H
#define DYNAMICSTENCIL_H

#include <memory>

#include <SDL2pp/SDL2pp.hh>

//#include "../../../../client/dtos/FOVInfo.h"
#include "../../../../common/dtos/FovConfig.h"

using SDL2pp::Renderer;
using SDL2pp::Texture;

class DynamicStencil {
private:
    static std::shared_ptr<Texture> stencil;  // TODO: quizas es mas acertado que sea un unique_ptr
    static bool isActive;
    static int screenWidth;                   // in pixels
    static int screenHeight;                  // in pixels
    static float cirRadius;
    static float fovAngle;  // in degrees
    static float fovRadius;
    static float transparency;
    static FOVConfig fovConfig;

    static void drawCircle(const Renderer& ren, float cx, float cy);
    static void drawFOVTriangle(const Renderer& ren, float cx, float cy);

public:
    DynamicStencil() = delete;

    ///
    //static void init(Renderer& ren, bool is_active, int screenW, int screenH, float circleRadius,
    //                 float fovAngleDegrees, uint8_t stencilAlpha, float fovVisibleDistance = 0);
    static void init(Renderer& ren, const FOVConfig &fov_config);

    static std::shared_ptr<Texture> getStencil();
    static FOVConfig getFOVConfig();
};

#endif  // DYNAMICSTENCIL_H
