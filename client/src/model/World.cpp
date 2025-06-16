#include "../../../client/include/model/World.h"

#include <vector>

#include "../../../client/include/model/EC/components/TransformComponent.h"

World::World(const TileMap& tileMap, const WindowConfig& winConfig, const int numPlayers,
             const LocalPlayerInfo& firstLocalPlayerSnap):
        entt_mgr(comp_mgr, numPlayers),
        comp_updater(entt_mgr, comp_mgr),
        map(tileMap),
        camera(winConfig.width, winConfig.height, tileMap.getColCount(), tileMap.getRowCount()),
        local_player(entt_mgr.create_local_player(firstLocalPlayerSnap)),
        render_sys(local_player) {}

void World::update(float dt, const GameInfo& gameInfo) {
    if (dt == 1) {}  // para que compile. Si no lo usamos sacar el parametro 'dt'
    gameInfo.print();
    const std::vector<EntitySnapshot> snapshots = gameInfo.getSnapshots();
    comp_updater.update(snapshots);

    player_HUD.updateFromSnapshot(gameInfo.localPlayer, gameInfo.timeLeft);

    // audio_sys.update(); // Play the sound effects.
}

void World::render(Graphics& graphics) {
    const auto tCompLocalPlayer = comp_mgr.getComponent<TransformComponent>(local_player);
    const Vec2D playerPos = tCompLocalPlayer->getPosition();
    camera.follow(playerPos);

    map.render(graphics, camera);

    player_FOV.render(graphics, playerPos, tCompLocalPlayer->getRotationAngle());

    render_sys.renderEntities(graphics, comp_mgr, camera, player_FOV);

    // player_HUD.render(graphics);
}

AimInfo World::getPlayerAimInfo(const int mouseX, const int mouseY) {
    Vec2D mouseWorldPos(static_cast<float>(mouseX), static_cast<float>(mouseY));
    // le sumo el offset de la camara para obtener las coordenadas del mouse
    // relativas al mundo (o al mapa) en vez de las relativas a la pantalla.
    mouseWorldPos += camera.getOffset();

    Vec2D aimDir = mouseWorldPos - getPlayerPosition();
    aimDir.normalize();
    const float angle = aimDir.calculateAngle();

    return {aimDir, angle};
}

Vec2D World::getPlayerPosition() {
    const auto tCompLocalPlayer = comp_mgr.getComponent<TransformComponent>(local_player);
    return tCompLocalPlayer->getPosition();
}
