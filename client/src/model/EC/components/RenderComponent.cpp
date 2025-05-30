#include "client/include/model/EC/components/RenderComponent.h"

using SDL2pp::Optional;
using SDL2pp::Rect;

void RenderComponent::render(Graphics& graphics, const SpriteComponent& sprComp,
                             const PositionComponent& posComp, const Camera& camera) {
    const Vec2D& enttMapPos = posComp.getPosition();  // Posición absoluta en el mapa.

    const int width = sprComp.getWidth();
    const int height = sprComp.getHeight();

    // Definimos el rectángulo de la entidad en el mapa (mundo)
    Rect entityRect(static_cast<int>(enttMapPos.getX()), static_cast<int>(enttMapPos.getY()), width,
                    height);

    const Rect& viewport = camera.getViewport();

    // Si la entidad está fuera del viewport, no la dibujamos.
    if (!viewport.Intersects(entityRect))
        return;

    // Calculamos la posición de la entidad relativa a la cámara
    Vec2D screenPos = enttMapPos - camera.getOffset();

    Rect destRect(static_cast<int>(screenPos.getX()), static_cast<int>(screenPos.getY()), width,
                  height);

    const double rotAngle = 0.0;  // TODO: reemplazar por rotación del TransformComponent

    graphics.draw(*sprComp.getTexture(), Optional<Rect>(sprComp.getSpriteRect()),
                  Optional<Rect>(destRect), rotAngle);
}
