#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "ECS/ComponentManager.h"
#include "ECS/EntityManager.h"
#include "ECS/System.h"
#include "ECS/systems/InputSystem.h"
#include "ECS/systems/RenderSystem.h"

using SDL2pp::Renderer;

class World {
private:
    EntityManager entt_mgr;
    ComponentManager comp_mgr;

    Entity local_player;  // This is the actual player that interacts with his own program.
    InputSystem input_sys;
    RenderSystem ren_sys;

    std::vector<std::unique_ptr<System>> comp_systems;

    std::vector<std::unique_ptr<System>> build_systems();

public:
    World();

    void update(float dt);
    void forward_event(const SDL_Event& e);
    void render(Renderer& ren);
};

#endif  // WORLD_H
