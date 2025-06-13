#include "client/include/model/World.h"

#include "client/include/model/EC/components/TransformComponent.h"

World::World(const TileMap& tileMap, const WindowConfig& winConfig, const int numPlayers,
             const EntitySnapshot& firstLocalPlayerSnap):
        entt_mgr(comp_mgr, numPlayers),
        comp_updater(entt_mgr, comp_mgr),
        map(tileMap),
        camera(winConfig.width, winConfig.height, tileMap.getColCount(), tileMap.getRowCount()),
        local_player(entt_mgr.create_entity(firstLocalPlayerSnap)) {}

void World::update(float dt, const std::vector<EntitySnapshot>& snapshots) {
    // std::cout << dt << std::endl;
    if (dt == 1) {}
    comp_updater.update(snapshots);
    // TODO: otra opcion para actualizar el HUD podria ser definiendo los componentes:
    // HealthComponent y MoneyComponent, y luego actualizar utilizando ComponentManager y
    // local_player_id. player_HUD.update(comp_mgr, local_player);
    for (const auto& snap: snapshots) {
        if (entt_mgr.get(snap.server_entt_id) == local_player) {
            player_HUD.updateFromSnapshot(snap);
            break;
        }
    }

    // audio_sys.update(); // Play the sound effects.
}

void World::render(Graphics& graphics) {
    const auto tCompLocalPlayer = comp_mgr.getComponent<TransformComponent>(local_player);
    const Vec2D playerPos = tCompLocalPlayer->getPosition();
    camera.follow(playerPos);

    map.render(graphics, camera);

    // Renderizar Field of View
    player_FOV.render(graphics, playerPos, tCompLocalPlayer->getRotationAngle());

    // TODO: Limitar el renderizado con usando el FOV.
    render_sys.renderEntities(graphics, comp_mgr, camera);

    player_HUD.render(graphics);
}

AimInfo World::getPlayerAimInfo(const int mouseX, const int mouseY) {
    Vec2D mouseWorldPos(static_cast<float>(mouseX), static_cast<float>(mouseY));
    // le sumo el offset de la camara para obtener las coordenadas del mouse
    // relativas al mundo (o al mapa) en vez de las relativas a la pantalla.
    mouseWorldPos += camera.getOffset();

    Vec2D aimDir = mouseWorldPos - getPlayerPosition();
    aimDir.normalize();
    const float angle = aimDir.calculateAngle(-90.0f);

    return {aimDir, angle};
}

Vec2D World::getPlayerPosition() {
    const auto tCompLocalPlayer = comp_mgr.getComponent<TransformComponent>(local_player);
    return tCompLocalPlayer->getPosition();
}
