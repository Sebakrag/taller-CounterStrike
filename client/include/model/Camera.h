#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2pp/SDL2pp.hh>

#include "utils/Vec2D.h"

using SDL2pp::Rect;

class Camera {
private:
    Vec2D offset;
    int viewportW, viewportH;  // in pixels.
    int mapWidth, mapHeight;   // in pixels.

public:
    Camera(int windowWidth, int windowHeight, int mapWidthInTiles, int mapHeightInTiles);

    void follow(const Vec2D& targetPosition);
    Vec2D getOffset() const;

    /// ///
    /// @return the visible area of the world (or map technically).
    /// ///
    Rect getViewport() const;  // TODO: Probably we dont need this.
};

#endif  // CAMERA_H
