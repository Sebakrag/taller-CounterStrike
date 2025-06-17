#ifndef FOVINFO_H
#define FOVINFO_H

///
/// @brief Encapsulates the information of the stencil required by the FieldOfView
///
struct FOVInfo {
    int screenWidth;   // in pixels
    int screenHeight;  // in pixels
    float fovAngle;    // in degrees
    float visibilityRadius;

    FOVInfo(const int screenWidth, const int screenHeight, const float fovAngle,
            const float fovRadius):
            screenWidth(screenWidth),
            screenHeight(screenHeight),
            fovAngle(fovAngle),
            visibilityRadius(fovRadius) {}
};

#endif  // FOVINFO_H
