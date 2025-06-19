#ifndef FOV_CONFIG_H_
#define FOV_CONFIG_H_

///
/// @brief Encapsulates the information of the stencil required by the FieldOfView
///

struct FOVConfig {
    bool isActive;
    int screenWidth;           // in pixels
    int screenHeight;          // in pixels
    int circleRadius;          // in pixels (stencil mask radius)
    float fovAngle;            // field of view angle in degrees (e.g., 90°)
    float visibilityDistance;  // maximum visible distance from player (in world units)
    float transparency;        // alpha value between 0 (fully transparent) and 1 (opaque)

    FOVConfig() = default;

    explicit FOVConfig(bool is_active, int screen_width, int screen_height, int circle_radius,
                       float fov_angle, float visibility_distance, float transparency):
            isActive(is_active),
            screenWidth(screen_width),
            screenHeight(screen_height),
            circleRadius(circle_radius),
            fovAngle(fov_angle),
            visibilityDistance(visibility_distance),
            transparency(transparency) {}
};

#endif  // FOV_CONFIG_H_
