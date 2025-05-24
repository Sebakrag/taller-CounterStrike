#ifndef COMPONENTUPDATER_H
#define COMPONENTUPDATER_H

#include <vector>

#include "client/dtos/EntitySnapshot.h"

#include "ComponentManager.h"
#include "EntityManager.h"


class ComponentUpdater {
private:
    struct OldEntityEntry {
        Entity e;
        EntitySnapshot snap;
    };
    EntityManager entt_mgr;
    ComponentManager comp_mgr;
    std::vector<OldEntityEntry>
            old_entities;  // entities that weren't created in the current frame.

public:
    ComponentUpdater() = default;

    ///
    /// @brief Synchronize all the entities that exists in the server (that comes in the snapshot).
    /// In order to do this, it creates or destroy entities as needed.
    /// @param snapshots vector that contains the info of each entity in the actual frame.
    ///
    void syncEntities(const std::vector<EntitySnapshot>& snapshots);

    ///
    /// @brief Set the values of each component of each entity that was not created in the actual
    /// frame.
    /// @param snapshots vector that contains the info of each entity in the actual frame.
    ///
    void applySnapshotData(const std::vector<EntitySnapshot>& snapshots);
    void updateComponents(const std::vector<EntitySnapshot>& snapshots, EntityManager& em,
                          ComponentManager& cm);
};


#endif  // COMPONENTUPDATER_H
