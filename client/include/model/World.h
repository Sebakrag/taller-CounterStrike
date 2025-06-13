#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "EC/ComponentManager.h"
#include "EC/ComponentUpdater.h"
#include "EC/EntityManager.h"
#include "EC/RenderSystem.h"
#include "client/dtos/AimInfo.h"
#include "common/dtos/EntitySnapshot.h"
#include "common/dtos/WindowConfig.h"

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

    Entity local_player;  // This is the actual player that interacts with his own program.
    HUD player_HUD;
    RenderSystem render_sys;

public:
    /// ///
    /// @param tileMap contains the info of the map selected by the creator of the game match.
    /// @param winConfig contains the info necessary to create the window for the game.
    /// @param numPlayers number of players that will play the game match.
    /// @param firstLocalPlayerSnap initial snapshot that the server send about our local_player.
    /// ///
    World(const TileMap& tileMap, const WindowConfig& winConfig, int numPlayers,
          const EntitySnapshot& firstLocalPlayerSnap);

    void update(float dt, const std::vector<EntitySnapshot>& snapshots);
    void render(Graphics& graphics);
    AimInfo getPlayerAimInfo(int mouseX, int mouseY);
    Vec2D getPlayerPosition();
};

#endif  // WORLD_H
