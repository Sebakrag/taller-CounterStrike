#include "client/include/model/World.h"

#include "client/include/model/EC/components/InputComponent.h"
#include "client/include/model/EC/components/PositionComponent.h"


World::World(const ServerEntityID& server_local_player_id):
        local_player(entt_mgr.create_entity(server_local_player_id)), input_handler(local_player) {
    // Adding components to the local_player
    comp_mgr.addComponent<InputComponent>(local_player);
    auto pos = comp_mgr.addComponent<PositionComponent>(local_player);
    pos->init(0, 0);
    // comp_mgr.addComponent(local_player, SpriteComponent());
}

void World::update(float dt) {
    for (const auto& comp_sys: comp_systems) comp_sys->update(dt, comp_mgr);
}

void World::forward_event(const SDL_Event& e) { input_handler.handle_event(e, comp_mgr); }

void World::render(Renderer& ren) { ren_sys.update(ren, comp_mgr); }


// void World::create_player(uint32_t id_player) {
//     // Somehow I need to sync the id of the player that comes from the server, to the id of
//     // the entity that it will represent the new player.
// }
