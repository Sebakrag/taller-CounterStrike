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

void World::handle_game_snapshot(const std::vector<EntitySnapshot>& snapshots) {
    comp_updater.syncEntities(snapshots);  // we create or destroy entities -> synchronizing
    comp_updater.applySnapshotData(
            snapshots);  // we only set the values of the components of the entities that
    // are not created in the actual frame.
}

void World::update(float dt, const std::vector<EntitySnapshot>& snapshots) {
    // if a snapshot carry the info of a new entity, the comp_updater should delegate the creation
    // of that entity to the EntityManager.
    // Probably the EntityManager could be responsible for the update of the component as well.
    // But I don't like this last idea (because EntityManger sounds more like a Factory to me).
    comp_updater.updateComponents(snapshots, entt_mgr, comp_mgr);
}

void World::forward_event(const SDL_Event& e) { input_handler.handle_event(e, comp_mgr); }

void World::render(Renderer& ren) { ren_sys.update(ren, comp_mgr); }


// void World::create_player(uint32_t id_player) {
//     // Somehow I need to sync the id of the player that comes from the server, to the id of
//     // the entity that it will represent the new player.
// }
