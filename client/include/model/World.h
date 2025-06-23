#ifndef WORLD_H
#define WORLD_H

#include "../../../client/dtos/AimInfo.h"
#include "../../../common/dtos/WindowConfig.h"
#include "../../../common/game_info/game_info.h"
#include "../../../common/tile_map.h"
#include "EC/AudioSystem.h"
#include "EC/ComponentManager.h"
#include "EC/ComponentUpdater.h"
#include "EC/EntityManager.h"
#include "EC/RenderSystem.h"

#include "Camera.h"
#include "FieldOfView.h"
#include "HUD.h"
#include "Map.h"

class Graphics;

class World {
private:
    ComponentManager comp_mgr;
    EntityManager entt_mgr;
    ComponentUpdater comp_updater;

    Map map;
    Camera camera;
    FieldOfView player_FOV;

    // TODO: podemos crear un struct para la info del localplayer.
    Entity local_player;  // This is the actual player that interacts with his own program.
    Weapon currentWeapon = Weapon::Knife;  // para consultar en EventHandler
    Vec2D local_player_pos;
    // Entity bomb; // TODO: Reconozco a la bomba?

    HUD player_HUD;
    RenderSystem render_sys;
    AudioSystem audio_sys;

public:
    /// ///
    /// @param tileMap contains the info of the map selected by the creator of the game match.
    /// @param winConfig contains the info necessary to create the window for the game.
    /// @param numPlayers number of players that will play the game match.
    /// @param firstLocalPlayerSnap initial snapshot that the server send about our local_player.
    /// ///
    World(Graphics& graphics, const TileMap& tileMap, const WindowConfig& winConfig, int numPlayers,
          const LocalPlayerInfo& firstLocalPlayerSnap);

    void update(float dt, const GameInfo& gameInfo);
    void render(Graphics& graphics);
    AimInfo getPlayerAimInfo(int mouseX, int mouseY);
    Vec2D getPlayerPosition() const;
};

#endif  // WORLD_H
