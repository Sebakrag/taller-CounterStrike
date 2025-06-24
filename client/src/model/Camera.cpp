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
    // newX = std::clamp(newX, 0.0f, static_cast<float>(mapWidth - viewportW));
    // newY = std::clamp(newY, 0.0f, static_cast<float>(mapHeight - viewportH));

    offset.set(newX, newY);
}

Vec2D Camera::getOffset() const { return offset; }

Rect Camera::getViewport() const {
    const int x = static_cast<int>(offset.getX());
    const int y = static_cast<int>(offset.getY());
    return {x, y, viewportW, viewportH};
}

bool Camera::isVisible(const Vec2D& enttMapPos, const int width, const int height) const {
    // Definimos el rectángulo de la entidad en el mapa (mundo)
    const Rect entityRect(static_cast<int>(enttMapPos.getX()), static_cast<int>(enttMapPos.getY()),
                          width, height);

    // Si la entidad está fuera del viewport, no la dibujamos.
    return getViewport().Intersects(entityRect);
}

Vec2D Camera::projectToScreen(const Vec2D& enttMapPos, const int width, const int height) const {
    Vec2D screenPos = enttMapPos - this->getOffset();
    // Corregimos el posicionamiento para centrar el sprite
    screenPos.setX(screenPos.getX() - static_cast<float>(width) / 2.0f);
    screenPos.setY(screenPos.getY() - static_cast<float>(height) / 2.0f);
    return screenPos;
}
