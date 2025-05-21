#ifndef VEC2D_H
#define VEC2D_H

#include <iostream>

class Vec2D {
private:
    float x;
    float y;
public:
    Vec2D();
    Vec2D(const float x, const float y);

    Vec2D& add(const Vec2D& vec);
    Vec2D& substract(const Vec2D& vec);
    Vec2D& multiply(const Vec2D& vec);
    Vec2D& divide(const Vec2D& vec);

    Vec2D& operator+=(const Vec2D& vec);
    Vec2D& operator-=(const Vec2D& vec);
    Vec2D& operator*=(const Vec2D& vec);
    Vec2D& operator/=(const Vec2D& vec);

    void reset();
    friend std::ostream& operator<<(std::ostream& stream, const Vec2D& vec);
};

#endif //VEC2D_H
