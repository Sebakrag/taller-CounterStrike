#ifndef COMPONENTUPDATER_H
#define COMPONENTUPDATER_H

#include <unordered_map>
#include <vector>

#include "../../../../common/dtos/EntitySnapshot.h"
#include "components/SoundComponent.h"

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

    static constexpr float MIN_MOVEMENT_EPSILON = 2.0f;

    struct PreviousPlayerInfo {
        PlayerState state;
        Entity weapon_id;
        Vec2D position;
        int health;

        PreviousPlayerInfo(const PlayerState s, const Entity weapon, const Vec2D& pos,
                           const int hp):
                state(s), weapon_id(weapon), position(pos), health(hp) {}
    };

    EntityManager& entt_mgr;
    ComponentManager& comp_mgr;
    // TODO: usar std::vector<std::pair<Entity, const EntitySnapshot*>>
    std::vector<OldEntityEntry>
            old_entities;  // entities that weren't created in the current frame.

    std::unordered_map<Entity, PreviousPlayerInfo> previous_player_info;

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
    void updateComponents();
    void updatePlayerSoundComponent(Entity e, SoundComponent& soundComp,
                                    const PlayerSnapshot& playerSnap, const Vec2D& curr_pos);

public:
    ComponentUpdater(EntityManager& em, ComponentManager& cm);

    void update(const std::vector<EntitySnapshot>& snapshots);
};


#endif  // COMPONENTUPDATER_H
