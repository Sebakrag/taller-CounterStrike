#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <SDL2pp/SDL2pp.hh>

#include "client/model/ECS/ComponentManager.h"

using SDL2pp::Renderer;

class RenderSystem {
public:
    RenderSystem() = default;
    void update(Renderer& ren, ComponentManager& cm);
};

#endif  // RENDERSYSTEM_H
