#include "../../../../client/include/model/utils/DynamicStencil.h"

#include <SDL2/SDL_render.h>

using SDL2pp::Surface;

std::shared_ptr<Texture> DynamicStencil::stencil;
bool DynamicStencil::isActive = false;
int DynamicStencil::screenWidth = 0;
int DynamicStencil::screenHeight = 0;
float DynamicStencil::cirRadius = 0.0f;
float DynamicStencil::fovAngle = 0.0f;  // in degrees
float DynamicStencil::fovRadius = 0.0f;
float DynamicStencil::transparency = 1.0f;

void DynamicStencil::init(Renderer& ren, const FOVConfig& config) {
    isActive = config.isActive;
    screenWidth = config.screenWidth;
    screenHeight = config.screenHeight;
    cirRadius = static_cast<float>(config.circleRadius);
    fovAngle = config.fovAngle;
    fovRadius = (config.visibilityDistance != 0.0f) ?
                        config.visibilityDistance :
                        static_cast<float>(std::max(screenWidth, screenHeight));
    transparency = config.transparency;

    const int stencilW = screenWidth * 2;
    const int stencilH = screenHeight * 2;
    stencil = std::make_shared<Texture>(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                        stencilW, stencilH);
    // Set render target to the texture
    ren.SetTarget(*stencil);
    ren.SetDrawColor(0, 0, 0, SDL_ALPHA_OPAQUE);  // Full black
    ren.Clear();

    // Center of the stencil texture
    const float cx = stencilW / 2.0f;
    const float cy = stencilH / 2.0f;

    // Transparent Circle (player vision center)
    ren.SetDrawColor(0, 0, 0, 0);
    drawCircle(ren, cx, cy);
    // Transparent Triangle (field of view)
    drawFOVTriangle(ren, cx, cy);

    uint8_t stencilAlpha = static_cast<uint8_t>(transparency * 255.0f);
    stencil->SetBlendMode(SDL_BLENDMODE_BLEND).SetAlphaMod(stencilAlpha);

    // Reset render target to default
    ren.SetTarget();
}

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
    float angleRad = 0.0f;  // Facing right
    float halfFOV = (fovAngle / 2.0f) * (M_PI / 180.0f);

    SDL_FPoint a = {cx, cy};
    SDL_FPoint b = {cx + fovRadius * std::cos(angleRad - halfFOV),
                    cy + fovRadius * std::sin(angleRad - halfFOV)};
    SDL_FPoint c = {cx + fovRadius * std::cos(angleRad + halfFOV),
                    cy + fovRadius * std::sin(angleRad + halfFOV)};

    SDL_Vertex triangle[3] = {{a, SDL_Color{0, 0, 0, 0}, SDL_FPoint{}},
                              {b, SDL_Color{0, 0, 0, 0}, SDL_FPoint{}},
                              {c, SDL_Color{0, 0, 0, 0}, SDL_FPoint{}}};

    int triangleIndices[3] = {0, 1, 2};

    SDL_RenderGeometry(ren.Get(), nullptr, triangle, 3, triangleIndices, 3);
}

std::shared_ptr<Texture> DynamicStencil::getStencil() { return stencil; }

FOVConfig DynamicStencil::getFOVConfig() {
    return FOVConfig(isActive, screenWidth, screenHeight,
                     static_cast<int>(cirRadius),  // circle radius
                     fovAngle,                     // FOV angle
                     fovRadius,                    // visibility distance
                     transparency);                // transparency [0,1]
}
