#include "client/include/model/World.h"

#include <iostream>

World::World(const EntitySnapshot& firstLocalPlayerSnap, const MapInfo& mapInfo):
        entt_mgr(comp_mgr),
        comp_updater(entt_mgr, comp_mgr),
        map(mapInfo),
        local_player(entt_mgr.create_entity(firstLocalPlayerSnap)),
        input_handler(local_player) {}

void World::handle_game_snapshot(const std::vector<EntitySnapshot>& snapshots) {
    comp_updater.syncEntities(snapshots);
    comp_updater.applySnapshotData();
}

void World::update(float dt) {
    std::cout << dt << std::endl;
    comp_updater.updateComponents();
}

void World::forward_event(const SDL_Event& e) { input_handler.handle_event(e, comp_mgr); }

void World::render(Renderer& ren) {
    map.render(
            ren);  // Le tendria que mandar la posicion del localPlayer para renderizar el jugador
    // en el medio, y para modificar la vista del mapa a medida que se mueve (camara)
    //  auto render_pool = comp_mgr.getPool<RenderComponent>();
    //  for (const auto comp : render_pool) {
    //      comp.update(ren);
    //  }
}
