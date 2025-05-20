#include "World.h"

#include "ECS/components/InputComponent.h"


World::World():
        local_player(entt_mgr.create_entity()),
        input_sys(local_player),
        comp_systems(build_systems()) {
    // Adding components to the local_player
    comp_mgr.addComponent(local_player, InputComponent());
    // comp_mgr.addComponent(player, SpriteComponent());
    // comp_mgr.addComponent(player, TransformComponent());
}

std::vector<std::unique_ptr<System>> World::build_systems() {
    // Estar atento al orden en que ubico los sistemas en el vector. Es importante!
    std::vector<std::unique_ptr<System>> vec;
    // vec.emplace_back(std::make_unique<MovementSystem>());
    return vec;
}

void World::update(float dt) {
    for (const auto& comp_sys: comp_systems) comp_sys->update(dt, comp_mgr);
}

void World::forward_event(const SDL_Event& e) { input_sys.handle_event(e, comp_mgr); }

void World::render(Renderer& ren) { ren_sys.update(ren, comp_mgr); }


// void World::create_player(uint32_t id_player) {
//     // Somehow I need to sync the id of the player that comes from the server, to the id of
//     // the entity that it will represent the new player.
// }
