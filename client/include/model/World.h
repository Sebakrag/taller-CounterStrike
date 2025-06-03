#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "EC/ComponentManager.h"
#include "EC/ComponentUpdater.h"
#include "EC/EntityManager.h"
#include "client/dtos/AimInfo.h"
#include "common/dtos/EntitySnapshot.h"
#include "common/dtos/MapInfo.h"
#include "common/dtos/WindowConfig.h"

#include "Camera.h"
#include "Map.h"

class Graphics;

class World {
private:
    ComponentManager comp_mgr;
    EntityManager entt_mgr;
    ComponentUpdater comp_updater;

    Map map;
    Camera camera;

    Entity local_player;  // This is the actual player that interacts with his own program.
    RenderSystem render_sys;

public:
    /// ///
    /// @param firstLocalPlayerSnap initial snapshot that the server send about our local_player.
    /// @param mapInfo contains the info of the map selected by the creator of the game match.
    /// ///
    World(const EntitySnapshot& firstLocalPlayerSnap, const MapInfo& mapInfo,
          const WindowConfig& winConfig);

    void update(float dt, const std::vector<EntitySnapshot>& snapshots);
    void render(Graphics& graphics);
    AimInfo getPlayerAimInfo(int mouseX, int mouseY);
};

#endif  // WORLD_H
