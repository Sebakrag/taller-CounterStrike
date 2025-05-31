#include "client/include/model/Camera.h"

#include "client/client_constants.h"


Camera::Camera(const int windowWidth, const int windowHeight, const int mapWidthInTiles,
               const int mapHeightInTiles):
        viewportW(windowWidth),
        viewportH(windowHeight),
        mapWidth(mapWidthInTiles * TILE_SIZE),
        mapHeight(mapHeightInTiles * TILE_SIZE) {}

void Camera::follow(const Vec2D& targetPosition) {
    float newX = targetPosition.getX() - (static_cast<float>(viewportW) / 2.0f);
    float newY = targetPosition.getY() - (static_cast<float>(viewportH) / 2.0f);

    // Clamp para que no se salga del mundo.
    newX = std::clamp(newX, 0.0f, static_cast<float>(mapWidth - viewportW));
    newY = std::clamp(newY, 0.0f, static_cast<float>(mapHeight - viewportH));

    offset.set(newX, newY);
}

Vec2D Camera::getOffset() const { return offset; }

Rect Camera::getViewport() const {
    const int x = static_cast<int>(offset.getX());
    const int y = static_cast<int>(offset.getY());
    return {x, y, viewportW, viewportH};
}
