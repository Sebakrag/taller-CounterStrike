#include "client/include/model/EC/components/RenderComponent.h"

#include <SDL2/SDL_render.h>

using SDL2pp::Optional;
using SDL2pp::Rect;

void RenderComponent::render(Graphics& graphics, const SpriteComponent& sprComp,
                             const PositionComponent& posComp) {
    const Vec2D position = posComp.getPosition();
    Rect onMapRect = {static_cast<int>(position.getX()), static_cast<int>(position.getY()),
                      sprComp.getWidth(), sprComp.getHeight()};

    const double rotAngle =
            0;  // Esto deberia agregarlo a un componente (quizas puedo crear TransformComponent)
    Optional<Rect> srcOpt(sprComp.getSpriteRect());
    Optional<Rect> dstOpt(onMapRect);
    graphics.draw(*sprComp.getTexture(), srcOpt, dstOpt, rotAngle);
}
