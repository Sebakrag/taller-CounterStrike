#include "client/include/model/World.h"

#include <iostream>

#include "client/include/model/EC/components/PositionComponent.h"

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
    const auto posLocalPlayer = comp_mgr.getComponent<PositionComponent>(local_player);
    std::cout << posLocalPlayer->getPosition() << std::endl;
    map.render(ren, posLocalPlayer->getPosition());  // Le tendria que mandar la posicion del
                                                     // localPlayer para renderizar el jugador
    // map.render(ren);
    // en el medio, y para modificar la vista del mapa a medida que se mueve (camara)
    //  auto render_pool = comp_mgr.getPool<RenderComponent>();
    //  for (const auto comp : render_pool) {
    //      comp.update(ren);
    //  }
}

void World::moveLocalPlayer(const Vec2D& dir) {
    const auto pos_comp = comp_mgr.getComponent<PositionComponent>(local_player);
    pos_comp->move(dir);  // Esto lo tengo que borrar (es solo para probar).
}
