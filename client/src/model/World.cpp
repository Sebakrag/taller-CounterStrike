#include "client/include/model/World.h"

#include <iostream>

#include "client/include/model/EC/components/TransformComponent.h"

World::World(const EntitySnapshot& firstLocalPlayerSnap, const MapInfo& mapInfo,
             const WindowConfig& winConfig):
        entt_mgr(comp_mgr),
        comp_updater(entt_mgr, comp_mgr),
        map(mapInfo),
        camera(winConfig.width, winConfig.height, mapInfo.numTilesInX, mapInfo.numTilesInY),
        local_player(entt_mgr.create_entity(firstLocalPlayerSnap)) {}

void World::update(float dt, const std::vector<EntitySnapshot>& snapshots) {
    std::cout << dt << std::endl;
    comp_updater.update(snapshots);
    // audio_sys.update(); // Play the sound effects.
}

void World::render(Graphics& graphics) {
    const auto tCompLocalPlayer = comp_mgr.getComponent<TransformComponent>(local_player);
    std::cout << tCompLocalPlayer->getPosition() << std::endl;
    camera.follow(tCompLocalPlayer->getPosition());

    map.render(graphics, camera);

    render_sys.renderEntities(graphics, comp_mgr, camera);

    // Renderizar el alpha blending (para simular el Field of View)

    // playerInventoryFrame.render(graphics); // Esto seria el frame que tiene la vida, la cant de
    // balas y la plata
}

AimInfo World::getPlayerAimInfo(const int mouseX, const int mouseY) {
    Vec2D mouseWorldPos(static_cast<float>(mouseX), static_cast<float>(mouseY));
    // le sumo el offset de la camara para obtener las coordenadas del mouse
    // relativas al mundo (o al mapa) en vez de las relativas a la pantalla.
    mouseWorldPos += camera.getOffset();

    const auto tCompLocalPlayer = comp_mgr.getComponent<TransformComponent>(local_player);

    Vec2D aimDir = mouseWorldPos - tCompLocalPlayer->getPosition();
    aimDir.normalize();
    const float angle = aimDir.calculateAngle(-90.0f);

    return {aimDir, angle};
}
