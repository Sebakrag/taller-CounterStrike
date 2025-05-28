#include "client/include/model/World.h"

#include <iostream>

#include "client/include/model/EC/components/PositionComponent.h"

World::World(const EntitySnapshot& firstLocalPlayerSnap, const MapInfo& mapInfo):
        entt_mgr(comp_mgr),
        comp_updater(entt_mgr, comp_mgr),
        map(mapInfo),
        local_player(entt_mgr.create_entity(firstLocalPlayerSnap)),
        input_handler(local_player) {}

void World::update(float dt, const std::vector<EntitySnapshot>& snapshots) {
    std::cout << dt << std::endl;
    comp_updater.update(snapshots);
}

void World::forwardEvent(const SDL_Event& e) { input_handler.handle_event(e, comp_mgr); }

void World::render(Graphics& graphics) {
    const auto posLocalPlayer = comp_mgr.getComponent<PositionComponent>(local_player);
    std::cout << posLocalPlayer->getPosition() << std::endl;
    map.render(graphics, posLocalPlayer->getPosition());
    // se renderizan los personajes, las armas y los objetos en si.
    //  auto render_pool = comp_mgr.getPool<RenderComponent>();
    //  for (const auto comp : render_pool) {
    //      comp.update(graphics);
    //  }
    // playerInventoryFrame.render(graphics); // Esto seria el frame que tiene la vida, la cant de
    // balas y la plata
}
