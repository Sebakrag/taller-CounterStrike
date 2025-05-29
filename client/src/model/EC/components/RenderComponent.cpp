#include "client/include/model/EC/components/RenderComponent.h"

#include <SDL2/SDL_render.h>

using SDL2pp::Optional;
using SDL2pp::Rect;

void RenderComponent::render(Graphics& graphics, const SpriteComponent& sprComp,
                             const PositionComponent& posComp, const Camera& camera) {
    Vec2D enttMapPos = posComp.getPosition();  // posicion absoluta de la entidad en el mapa.

    const Vec2D enttScreenPos = enttMapPos.substract(
            camera.getOffset());  // posicion de la entidad relativa al viewport de la camara.

    Rect onScreenRect = {static_cast<int>(enttScreenPos.getX()),
                         static_cast<int>(enttScreenPos.getY()), sprComp.getWidth(),
                         sprComp.getHeight()};

    const double rotAngle =
            0;  // Esto deberia agregarlo a un componente (quizas puedo crear TransformComponent)

    Optional<Rect> srcOpt(sprComp.getSpriteRect());
    Optional<Rect> dstOpt(onScreenRect);
    graphics.draw(*sprComp.getTexture(), srcOpt, dstOpt, rotAngle);
}

//
// void RenderComponent::render(Graphics& graphics, const SpriteComponent& sprComp,
//                              const PositionComponent& posComp, const Camera& camera) {
//     Vec2D enttMapPos = posComp.getPosition(); // posicion absoluta de la entidad en el mapa.
//
//     Rect viewport = camera.getViewport();
//
//     if (viewport.Intersects(entityRect)) {
//         const Vec2D enttScreenPos = enttMapPos.substract(camera.getOffset()); // posicion de la
//         entidad relativa al viewport de la camara.
//
//         Rect onScreenRect = {static_cast<int>(enttScreenPos.getX()),
//         static_cast<int>(enttScreenPos.getY()),
//                           sprComp.getWidth(), sprComp.getHeight()};
//
//         const double rotAngle =
//                 0;  // Esto deberia agregarlo a un componente (quizas puedo crear
//                 TransformComponent)
//
//         Optional<Rect> srcOpt(sprComp.getSpriteRect());
//         Optional<Rect> dstOpt(onScreenRect);
//         graphics.draw(*sprComp.getTexture(), srcOpt, dstOpt, rotAngle);
//     }
// }
