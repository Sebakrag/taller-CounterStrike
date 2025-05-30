#include "client/include/model/utils/Vec2D.h"


Vec2D::Vec2D(): x(0.0f), y(0.0f) {}

Vec2D::Vec2D(const float x, const float y): x(x), y(y) {}

Vec2D& Vec2D::add(const Vec2D& other) {
    this->x += other.x;
    this->y += other.y;

    return *this;
}

Vec2D& Vec2D::substract(const Vec2D& other) {
    this->x -= other.x;
    this->y -= other.y;

    return *this;
}

Vec2D& Vec2D::multiply(const Vec2D& other) {
    this->x *= other.x;
    this->y *= other.y;

    return *this;
}

Vec2D& Vec2D::divide(const Vec2D& other) {
    this->x /= other.x;
    this->y /= other.y;

    return *this;
}

Vec2D& Vec2D::operator+=(const Vec2D& other) { return this->add(other); }

Vec2D& Vec2D::operator-=(const Vec2D& other) { return this->substract(other); }

Vec2D& Vec2D::operator*=(const Vec2D& other) { return this->multiply(other); }

Vec2D& Vec2D::operator/=(const Vec2D& other) { return this->divide(other); }

Vec2D Vec2D::operator+(const Vec2D& other) const { return {this->x + other.x, this->y + other.y}; }

Vec2D Vec2D::operator-(const Vec2D& other) const { return {this->x - other.x, this->y - other.y}; }

Vec2D Vec2D::operator*(const Vec2D& other) const { return {this->x * other.x, this->y * other.y}; }

Vec2D Vec2D::operator/(const Vec2D& other) const { return {this->x / other.x, this->y / other.y}; }


void Vec2D::set(const float x, const float y) {
    setX(x);
    setY(y);
}

void Vec2D::setX(const float x) { this->x = x; }

void Vec2D::setY(const float y) { this->y = y; }

float Vec2D::getX() const { return x; }

float Vec2D::getY() const { return y; }

std::ostream& operator<<(std::ostream& stream, const Vec2D& vec) {
    stream << "(" << vec.x << "," << vec.y << ")";
    return stream;
}
