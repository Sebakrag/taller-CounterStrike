#include "client/include/model/World.h"

#include <iostream>

#include "client/include/model/EC/components/PositionComponent.h"
#include "client/include/model/EC/components/RenderComponent.h"

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
}

void World::render(Graphics& graphics) {
    const auto posLocalPlayer = comp_mgr.getComponent<PositionComponent>(local_player);
    std::cout << posLocalPlayer->getPosition() << std::endl;
    camera.follow(posLocalPlayer->getPosition());

    map.render(graphics, camera);

    // se renderizan los personajes, las armas y los objetos en si:
    comp_mgr.forEach<RenderComponent>([&](RenderComponent& renderComp, const Entity e) {
        const auto sprite = comp_mgr.getComponent<SpriteComponent>(e);
        const auto pos = comp_mgr.getComponent<PositionComponent>(e);

        if (sprite && pos) {
            renderComp.render(graphics, *sprite, *pos, camera);
        }
    });

    // Renderizar el alpha blending (para simular el Field of View)

    // playerInventoryFrame.render(graphics); // Esto seria el frame que tiene la vida, la cant de
    // balas y la plata
}

AimInfo World::getPlayerAimInfo(int mouseX, int mouseY) {
    Vec2D mouseWorldPos(mouseX, mouseY);
    // le sumo el offset de la camara para obtener las coordenadas del mouse
    // relativas al mundo (o al mapa) en vez de las relativas a la pantalla.
    mouseWorldPos += camera.getOffset();

    const auto posLocalPlayer = comp_mgr.getComponent<PositionComponent>(local_player);

    Vec2D aimDir = mouseWorldPos - posLocalPlayer->getPosition();
    aimDir.normalize();
    const float angle = aimDir.calculateAngle();

    return {aimDir, angle};
}
