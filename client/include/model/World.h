#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "EC/ComponentManager.h"
#include "EC/ComponentUpdater.h"
#include "EC/EntityManager.h"
#include "client/dtos/EntitySnapshot.h"
#include "utils/InputHandler.h"

using SDL2pp::Renderer;

class World {
private:
    EntityManager entt_mgr;
    ComponentManager comp_mgr;
    ComponentUpdater comp_updater;

    Entity local_player;  // This is the actual player that interacts with his own program.
    InputHandler input_handler;
    // RenderComponent ren_comp;  // I believe this is not necessary anymore.

public:
    /// ///
    /// @param firstLocalPlayerSnap initial snapshot that the server send about our local_player.
    /// ///
    explicit World(const EntitySnapshot& firstLocalPlayerSnap);

    void handle_game_snapshot(const std::vector<EntitySnapshot>& snapshots);
    void update(float dt);
    void forward_event(const SDL_Event& e);
    void render(Renderer& ren);
};

#endif  // WORLD_H
