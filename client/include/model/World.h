#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "EC/ComponentManager.h"
#include "EC/ComponentUpdater.h"
#include "EC/EntityManager.h"
#include "client/dtos/EntitySnapshot.h"
#include "client/dtos/MapInfo.h"

#include "Map.h"

class Graphics;

class World {
private:
    ComponentManager comp_mgr;
    EntityManager entt_mgr;
    ComponentUpdater comp_updater;

    Map map;

    Entity local_player;  // This is the actual player that interacts with his own program.
    // RenderComponent ren_comp;  // I believe this is not necessary anymore.

public:
    /// ///
    /// @param firstLocalPlayerSnap initial snapshot that the server send about our local_player.
    /// @param mapInfo contains the info of the map selected by the creator of the game match.
    /// ///
    World(const EntitySnapshot& firstLocalPlayerSnap, const MapInfo& mapInfo);

    void update(float dt, const std::vector<EntitySnapshot>& snapshots);
    void render(Graphics& graphics);
};

#endif  // WORLD_H
