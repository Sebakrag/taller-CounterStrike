#ifndef COMPONENTUPDATER_H
#define COMPONENTUPDATER_H

#include <vector>

#include "client/dtos/EntitySnapshot.h"

#include "ComponentManager.h"
#include "EntityManager.h"

#define INITIAL_OLD_ENTITIES_SIZE 256

class ComponentUpdater {
private:
    struct OldEntityEntry {
        Entity e;
        EntitySnapshot snap;

        OldEntityEntry(const Entity e, const EntitySnapshot& snap): e(e), snap(snap) {}
    };
    EntityManager& entt_mgr;
    ComponentManager& comp_mgr;
    std::vector<OldEntityEntry>
            old_entities;  // entities that weren't created in the current frame.

public:
    ComponentUpdater(EntityManager& em, ComponentManager& cm);

    ///
    /// @brief Synchronize all the entities that exists in the server (that comes in the snapshot).
    /// In order to do this, it creates or destroy entities as needed.
    /// @param snapshots vector that contains the info of each entity in the current frame.
    ///
    void syncEntities(const std::vector<EntitySnapshot>& snapshots);

    ///
    /// @brief Set the values of each component of each entity that was not created in the current
    /// frame.
    /// @note This function should be call right after the method "syncEntities". If not, an
    /// unknown behavior could take place.
    ///
    void applySnapshotData();
    void updateComponents();
};


#endif  // COMPONENTUPDATER_H
