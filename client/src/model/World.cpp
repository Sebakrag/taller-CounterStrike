#include "../../../client/include/model/World.h"

#include "../../../client/include/model/EC/components/TransformComponent.h"

World::World(Graphics& graphics, Audio& audio, const TileMap& tileMap,
             const WindowConfig& winConfig, const int numPlayers,
             const LocalPlayerInfo& firstLocalPlayerSnap):
        graphics(graphics),
        gamePhase(GamePhase::Preparation),
        entt_mgr(comp_mgr, numPlayers),
        comp_updater(entt_mgr, comp_mgr),
        map(tileMap, graphics),
        camera(winConfig.width, winConfig.height, tileMap.getColCount(), tileMap.getRowCount()),
        local_player(entt_mgr.create_local_player(firstLocalPlayerSnap)),
        // bomb(entt_mgr.create_bomb(firstBombSnap)),
        render_sys(local_player),
        audio_sys(comp_mgr, audio) {}


void World::update(const GameInfo& gameInfo, const float dt) {
    if (dt == 1) {}  // TODO: dt sirve para las animaciones.
    gamePhase = gameInfo.gamePhase;
    if (gamePhase == GamePhase::Combat) {
        currentWeapon = gameInfo.localPlayer.weapon;
        local_player_pos = gameInfo.localPlayer.position;

        // comp_updater.update(gameInfo.getSnapshots());
        comp_updater.update(gameInfo.getSnapshots(), gameInfo.timeLeft);

        audio_sys.update(local_player_pos);  // Play the sound effects.
    }

    player_HUD.updateFromSnapshot(gameInfo.localPlayer, gameInfo.timeLeft);
}

void World::render() {
    // TODO: podriamos guardar el angulo de rotacion del player como miembro de World asi
    /// desacoplamos a World completamente de TransformComponent.
    /// Atencion: el angulo tiene que estar en degrees para el renderizado!!
    const auto tCompLocalPlayer = comp_mgr.getComponent<TransformComponent>(local_player);

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
