#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "client/include/model/Camera.h"
#include "client/include/model/Graphics.h"

#include "PositionComponent.h"
#include "SpriteComponent.h"

class RenderComponent {
public:
    RenderComponent() = default;

    void render(Graphics& graphics, const SpriteComponent& sprComp,
                const PositionComponent& posComp, const Camera& camera);
};

#endif  // RENDERCOMPONENT_H
