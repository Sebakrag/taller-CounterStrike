#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2pp/SDL2pp.hh>

#include "common/utils/Vec2D.h"

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

    /// ///
    /// @brief Verifica si el sprite de una entidad es visible por la camra del jugador
    /// o no. Retorna true en caso afirmativo, o false en caso contrario.
    ///
    /// @param enttMapPos Posicion absoluta de la entidad en el mapa (o mundo).
    /// @param width Ancho del sprite de la entidad a renderizar.
    /// @param height Alto del sprite de la entidad a renderizar.
    /// ///
    bool isVisible(const Vec2D& enttMapPos, int width, int height) const;

    /// ///
    /// @ brief Crea un vector que representa la posicion de la entidad en el mundo proyectada a
    /// la pantalla visible por el jugador.
    ///
    /// @param enttMapPos Posicion absoluta de la entidad en el mapa (o mundo).
    /// @param width Ancho del sprite de la entidad a renderizar.
    /// @param height Alto del sprite de la entidad a renderizar.
    /// ///
    Vec2D projectToScreen(const Vec2D& enttMapPos, int width, int height) const;
};

#endif  // CAMERA_H
