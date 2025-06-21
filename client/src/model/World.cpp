#include "../../../client/include/model/World.h"

#include "../../../client/include/model/EC/components/TransformComponent.h"

World::World(Graphics& graphics, const TileMap& tileMap, const WindowConfig& winConfig,
             const int numPlayers, const LocalPlayerInfo& firstLocalPlayerSnap):
        entt_mgr(comp_mgr, numPlayers),
        comp_updater(entt_mgr, comp_mgr),
        map(tileMap, graphics),
        camera(winConfig.width, winConfig.height, tileMap.getColCount(), tileMap.getRowCount()),
        local_player(entt_mgr.create_local_player(firstLocalPlayerSnap)),
        render_sys(local_player),
        audio_sys(comp_mgr, graphics.getMixer()) {}


void World::update(float dt, const GameInfo& gameInfo) {
    if (dt == 1) {}  // para que compile. Si no lo usamos sacar el parametro 'dt'
    // gameInfo.print();
    currentWeapon = gameInfo.localPlayer.weapon;
    local_player_pos = gameInfo.localPlayer.position;

    comp_updater.update(gameInfo.getSnapshots());

    player_HUD.updateFromSnapshot(gameInfo.localPlayer, gameInfo.timeLeft);

    audio_sys.update(local_player_pos);  // Play the sound effects.
}

void World::render(Graphics& graphics) {
    const auto tCompLocalPlayer = comp_mgr.getComponent<TransformComponent>(local_player);
    // std::cout << playerPos << std::endl;
    camera.follow(local_player_pos);

    map.render2(graphics, camera);

    player_FOV.render(graphics, local_player_pos, tCompLocalPlayer->getRotationAngleDegrees());

    render_sys.renderEntities(graphics, comp_mgr, camera, player_FOV);

    player_HUD.render(graphics);
}

AimInfo World::getPlayerAimInfo(const int mouseX, const int mouseY) {
    Vec2D mouseWorldPos(static_cast<float>(mouseX), static_cast<float>(mouseY));
    // le sumo el offset de la camara para obtener las coordenadas del mouse
    // relativas al mundo (o al mapa) en vez de las relativas a la pantalla.
    mouseWorldPos += camera.getOffset();

    Vec2D aimDir = mouseWorldPos - local_player_pos;
    aimDir.normalize();
    const float angle = aimDir.calculateAngleDegrees();

    return {aimDir, angle, currentWeapon};
}
