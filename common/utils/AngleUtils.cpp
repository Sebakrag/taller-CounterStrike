#include "AngleUtils.h"

namespace AngleUtils {
    float normalizeRadians_0_2PI(float radians) {
        radians = std::fmod(radians, TWO_PI);
        if (radians < ZERO_RAD)
            radians += TWO_PI;
        return radians;
    }

    float normalizeRadians_PI(float radians) {
        radians = std::fmod(radians + PI, TWO_PI);
        if (radians < ZERO_RAD)
            radians += TWO_PI;
        return radians - PI;
    }

    float normalizeDegrees_0_360(float degrees) {
        degrees = std::fmod(degrees, DEG_360);
        if (degrees < ZERO_DEG)
            degrees += DEG_360;
        return degrees;
    }

    float normalizeDegrees_180(float degrees) {
        degrees = std::fmod(degrees + DEG_180, DEG_360);
        if (degrees < ZERO_DEG)
            degrees += DEG_360;
        return degrees - DEG_180;
    }

    float degreesToRadians(const float degrees) {
        return degrees * (PI / DEG_180);
    }

    float radiansToDegrees(const float radians) {
        return radians * (DEG_180 / PI);
    }
}
