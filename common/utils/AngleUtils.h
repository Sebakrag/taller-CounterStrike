#ifndef ANGLEUTILS_H
#define ANGLEUTILS_H

#include <cmath>

namespace AngleUtils {
    inline constexpr float ZERO_RAD = 0.0f;
    inline constexpr float TWO_PI = 2.0f * static_cast<float>(M_PI);
    inline constexpr float PI = M_PI;

    inline constexpr float ZERO_DEG = 0.0f;
    inline constexpr float DEG_180 = 180.0f;
    inline constexpr float DEG_360 = 360.0f;

    // ----- Radianes -----
    ////
    /// @brief Normaliza un angulo en radianes al rango [0, 2π).
    ///
    /// @param radians Angulo en radianes.
    /// @return Angulo equivalente en el rango [0, 2π).
    ////
    float normalizeRadians_0_2PI(float radians);

    ////
    /// @brief Normaliza un angulo en radianes al rango [-π, π).
    /// Util para comparar diferencias angulares.
    /// @param radians Angulo en radianes.
    /// @return Angulo equivalente en el rango [-π, π).
    ////
    float normalizeRadians_PI(float radians);

    // ----- Grados -----
    ////
    /// @brief Normaliza un angulo en grados al rango [0, 360).
    /// @param degrees Angulo en grados.
    /// @return Angulo equivalente en el rango [0, 360).
    ////
    float normalizeDegrees_0_360(float degrees);

    ////
    /// @brief Normaliza un angulo en grados al rango [-180, 180).
    /// Util para comparar diferencias angulares.
    /// @param degrees Angulo en grados.
    /// @return Angulo equivalente en el rango [-180, 180).
    ////
    float normalizeDegrees_180(float degrees);

    // ----- Conversión -----
    ////
    /// @brief Convierte un angulo en grados a radianes.
    /// @param degrees Angulo en grados.
    /// @return Angulo equivalente en radianes.
    ////
    float degreesToRadians(float degrees);

    ////
    /// @brief Convierte un angulo en radianes a grados.
    /// @param radians Angulo en radianes.
    /// @return Angulo equivalente en grados.
    ////
    float radiansToDegrees(float radians);
}

#endif //ANGLEUTILS_H
