#ifndef VEC2D_H
#define VEC2D_H

#include <iostream>

class Vec2D {
private:
    float x;
    float y;

    Vec2D& add(const Vec2D& other);
    Vec2D& substract(const Vec2D& other);
    Vec2D& multiply(const Vec2D& other);
    Vec2D& divide(const Vec2D& other);

public:
    Vec2D();
    Vec2D(float x, float y);
    Vec2D(const Vec2D& other) = default;
    Vec2D& operator=(const Vec2D& other) = default;

    Vec2D& operator+=(const Vec2D& other);
    Vec2D& operator-=(const Vec2D& other);
    Vec2D& operator*=(const Vec2D& other);
    Vec2D& operator/=(const Vec2D& other);

    Vec2D operator+(const Vec2D& other) const;
    Vec2D operator-(const Vec2D& other) const;
    Vec2D operator*(const Vec2D& other) const;
    Vec2D operator/(const Vec2D& other) const;

    float dot(const Vec2D& other) const;

    Vec2D& normalize();
    ///
    /// @brief calculates the angle (in degrees) relative to the reference system
    /// commonly used in graphics
    ///
    float calculateAngleDegrees(float correctionDegrees = 0.0f) const;

    ///
    /// @brief calculates the angle (in radian) returning a value in the range [-π, π]
    ///
    float calculateAngleRadian() const;
    float calculateNormSquared() const;

    void set(float x, float y);
    void setX(float x);
    void setY(float y);
    float getX() const;
    float getY() const;
    friend std::ostream& operator<<(std::ostream& stream, const Vec2D& vec);
};

#endif  // VEC2D_H
