#include "../../../client/include/model/World.h"

#include <iostream>

#include "../../../client/include/model/EC/components/RenderComponent.h"
#include "../../../client/include/model/EC/components/TransformComponent.h"

World::World(const EntitySnapshot& firstLocalPlayerSnap, const TileMap& tileMap,
             const WindowConfig& winConfig):
        entt_mgr(comp_mgr),
        comp_updater(entt_mgr, comp_mgr),
        map(tileMap),
        camera(winConfig.width, winConfig.height, tileMap.getColCount(), tileMap.getRowCount()),
        local_player(entt_mgr.create_entity(firstLocalPlayerSnap)) {}

void World::update(float dt, const std::vector<EntitySnapshot>& snapshots) {
    std::cout << dt << std::endl;
    comp_updater.update(snapshots);
}

void World::render(Graphics& graphics) {
    const auto tCompLocalPlayer = comp_mgr.getComponent<TransformComponent>(local_player);
    // std::cout << tCompLocalPlayer->getPosition() << std::endl;
    camera.follow(tCompLocalPlayer->getPosition());

    map.render(graphics, camera);

    // se renderizan los personajes, las armas y los objetos en si:
    comp_mgr.forEach<RenderComponent>([&](RenderComponent& renderComp, const Entity e) {
        const auto sprite = comp_mgr.getComponent<SpriteComponent>(e);
        const auto transform = comp_mgr.getComponent<TransformComponent>(e);

        if (sprite && transform) {
            renderComp.render(graphics, *sprite, *transform, camera);
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

    const auto tCompLocalPlayer = comp_mgr.getComponent<TransformComponent>(local_player);

    Vec2D aimDir = mouseWorldPos - tCompLocalPlayer->getPosition();
    aimDir.normalize();
    const float angle = aimDir.calculateAngle(-90.0f);

    return {aimDir, angle};
}
