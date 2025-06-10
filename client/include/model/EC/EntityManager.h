#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <unordered_map>
#include <unordered_set>

#include "common/dtos/EntitySnapshot.h"

#include "ComponentManager.h"
#include "Entity.h"
#include "EntityFactory.h"

using ServerEntityID = uint32_t;

class EntityManager {
private:
    Entity next_id = 1;
    std::unordered_set<Entity> free_ids;
    std::unordered_map<ServerEntityID, Entity> server_entt_id_to_entity;

    EntityFactory entt_factory;

public:
    EntityManager(ComponentManager& cm, int numPlayers);

    Entity create_entity(const EntitySnapshot& snap);

    ///
    /// @brief Destroy the entity that corresponds to the id received from the server.
    /// It destroys all the components associated with the entity as well.
    ///
    /// @param serv_entt_id ID that the server assign to the entity.
    ///
    /// @note This function should be call for those entities that should do nothing after
    /// "death".
    ///
    void destroy_entity_immediately(const ServerEntityID& serv_entt_id);

    Entity get(const ServerEntityID& serv_entt_id) const;
};

#endif  // ENTITYMANAGER_H
