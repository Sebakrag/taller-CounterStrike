#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include <SDL2pp/SDL2pp.hh>

#include "client/include/model/Graphics.h"

#include "PositionComponent.h"
#include "SpriteComponent.h"

// using SDL2pp::Point;

class RenderComponent {
private:
    // Point origin; // origen relativo a dstRect a partir de donde la imagen rota (quizas no es
    // necesario).

public:
    RenderComponent() = default;

    void render(Graphics& graphics, const SpriteComponent& sprComp,
                const PositionComponent& posComp);
};

#endif  // RENDERCOMPONENT_H
