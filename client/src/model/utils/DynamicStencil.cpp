#include "client/include/model/utils/DynamicStencil.h"

#include <SDL2/SDL_render.h>

using SDL2pp::Surface;

std::shared_ptr<Texture> DynamicStencil::stencil;
int DynamicStencil::screenWidth;
int DynamicStencil::screenHeight;
float DynamicStencil::cirRadius;
float DynamicStencil::fovAngle;  // in degrees
float DynamicStencil::fovRadius;

void DynamicStencil::init(Renderer& ren, const int screenW, const int screenH,
                          const float circleRadius, const float fovAngleDegrees,
                          const uint8_t stencilAlpha, const float fovVisibleDistance) {
    screenWidth = screenW;
    screenHeight = screenH;
    const int stencilW = screenW * 2;
    const int stencilH = screenH * 2;
    stencil = std::make_shared<Texture>(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                        stencilW, stencilH);
    cirRadius = circleRadius;
    fovAngle = fovAngleDegrees;
    fovRadius = fovVisibleDistance;

    // Set render target to the texture
    ren.SetTarget(*stencil);
    ren.SetDrawColor(0, 0, 0, SDL_ALPHA_OPAQUE);  // Full black
    ren.Clear();

    // Center of the stencil texture
    const float cx = stencilW / 2.0f;
    const float cy = stencilH / 2.0f;

    // TODO: si no funciona asi, tendria que usar Surface con color key. (ver el repo de clases.)
    // Transparent Circle (player vision center)
    ren.SetDrawColor(0, 0, 0, 0);
    drawCircle(ren, cx, cy);
    // Transparent Triangle (field of view)
    drawFOVTriangle(ren, cx, cy);

    stencil->SetBlendMode(SDL_BLENDMODE_BLEND).SetAlphaMod(stencilAlpha);

    // Reset render target to default
    ren.SetTarget();
}


// void DynamicStencil::init(Renderer& ren, const int screenW, const int screenH, const float
// circleRadius, const float fovAngleDegrees, const uint8_t stencilAlpha, const float
// fovVisibleDistance) {
//     screenWidth = screenW * 2;
//     screenHeight = screenH * 2;
//     stencil = std::make_shared<Texture>(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
//     screenWidth, screenHeight);
//     stencil->SetBlendMode(SDL_BLENDMODE_BLEND).SetAlphaMod(stencilAlpha);
//     cirRadius = circleRadius;
//     fovAngle = fovAngleDegrees;
//     fovRadius = fovVisibleDistance;
//
//     // Set render target to the texture
//     ren.SetTarget(*stencil);
//     ren.SetDrawColor(0, 0, 0, 255);  // Full black (shadow)
//     ren.Clear();
//
//     // Center of the screen
//     const float cx = screenW / 2.0f;
//     const float cy = screenH / 2.0f;
//
//     // TODO: si no funciona asi, tendria que usar Surface con color key. (ver el repo de clases.)
//     // Transparent Circle (player vision center)
//     ren.SetDrawColor(0, 0, 0, 0);
//     drawCircle(ren, cx, cy);
//
//     // Transparent Triangle (field of view)
//     ren.SetDrawColor(0, 0, 0, 0);
//     drawFOVTriangle(ren, cx, cy);
//
//     // Reset render target to default
//     ren.SetTarget();
// }

void DynamicStencil::drawCircle(const Renderer& ren, const float cx, const float cy) {
    constexpr int numSegments = 100;
    std::vector<SDL_Vertex> circleVertices;
    circleVertices.push_back(SDL_Vertex{SDL_FPoint{cx, cy}, SDL_Color{0, 0, 0, 0}, SDL_FPoint{}});

    for (int i = 0; i <= numSegments; ++i) {
        float theta = i * 2.0f * M_PI / numSegments;
        float x = cx + cirRadius * std::cos(theta);
        float y = cy + cirRadius * std::sin(theta);

        circleVertices.push_back(SDL_Vertex{SDL_FPoint{x, y}, SDL_Color{0, 0, 0, 0}, SDL_FPoint{}});
    }

    std::vector<int> circleIndices;
    for (int i = 1; i <= numSegments; ++i) {
        circleIndices.push_back(0);
        circleIndices.push_back(i);
        circleIndices.push_back(i + 1);
    }

    SDL_RenderGeometry(ren.Get(), nullptr, circleVertices.data(),
                       static_cast<int>(circleVertices.size()), circleIndices.data(),
                       static_cast<int>(circleIndices.size()));
}

void DynamicStencil::drawFOVTriangle(const Renderer& ren, const float cx, const float cy) {
    float radius =
            (fovRadius != 0) ? fovRadius : static_cast<float>(std::max(screenWidth, screenHeight));
    float angleRad = 0.0f;  // Facing right
    float halfFOV = (fovAngle / 2.0f) * (M_PI / 180.0f);

    SDL_FPoint a = {cx, cy};
    SDL_FPoint b = {cx + radius * std::cos(angleRad - halfFOV),
                    cy + radius * std::sin(angleRad - halfFOV)};
    SDL_FPoint c = {cx + radius * std::cos(angleRad + halfFOV),
                    cy + radius * std::sin(angleRad + halfFOV)};

    SDL_Vertex triangle[3] = {{a, SDL_Color{0, 0, 0, 0}, SDL_FPoint{}},
                              {b, SDL_Color{0, 0, 0, 0}, SDL_FPoint{}},
                              {c, SDL_Color{0, 0, 0, 0}, SDL_FPoint{}}};

    int triangleIndices[3] = {0, 1, 2};

    SDL_RenderGeometry(ren.Get(), nullptr, triangle, 3, triangleIndices, 3);
}

std::shared_ptr<Texture> DynamicStencil::getStencil() { return stencil; }

FOVInfo DynamicStencil::getFOVInfo() { return {screenWidth, screenHeight, fovAngle, fovRadius}; }
