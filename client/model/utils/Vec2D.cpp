#include "Vec2D.h"


Vec2D::Vec2D() : x(0.0f), y(0.0f) {}

Vec2D::Vec2D(const float x, const float y) : x(x), y(y) {}

Vec2D& Vec2D::add(const Vec2D& vec) {
    this->x += vec.x;
    this->y += vec.y;

    return *this;
}

Vec2D& Vec2D::substract(const Vec2D& vec) {
    this->x -= vec.x;
    this->y -= vec.y;

    return *this;
}

Vec2D& Vec2D::multiply(const Vec2D& vec) {
    this->x *= vec.x;
    this->y *= vec.y;

    return *this;
}

Vec2D& Vec2D::divide(const Vec2D& vec) {
    this->x /= vec.x;
    this->y /= vec.y;

    return *this;
}

Vec2D& Vec2D::operator+=(const Vec2D& vec) {
    return this->add(vec);
}

Vec2D& Vec2D::operator-=(const Vec2D& vec) {
    return this->substract(vec);
}

Vec2D& Vec2D::operator*=(const Vec2D& vec) {
    return this->multiply(vec);
}

Vec2D& Vec2D::operator/=(const Vec2D& vec) {
    return this->divide(vec);
}

void Vec2D::reset() {
    this->x = 0.0f;
    this->y = 0.0f;
}

std::ostream& operator<<(std::ostream& stream, const Vec2D& vec) {
    stream << "(" << vec.x << "," << vec.y << ")" << std::endl;
    return stream;
}
