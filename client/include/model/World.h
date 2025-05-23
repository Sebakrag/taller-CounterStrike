#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "EC/ComponentManager.h"
#include "EC/EntityManager.h"
#include "utils/InputHandler.h"

using SDL2pp::Renderer;

class World {
private:
    EntityManager entt_mgr;
    ComponentManager comp_mgr;

    Entity local_player;  // This is the actual player that interacts with his own program.
    InputHandler input_handler;
    // RenderComponent ren_comp;

public:
    /// ///
    /// @param server_local_player_id ID that the server gives to our local_player.
    /// ///
    explicit World(const ServerEntityID& server_local_player_id);

    void update(float dt);
    void forward_event(const SDL_Event& e);
    void render(Renderer& ren);
};

#endif  // WORLD_H
