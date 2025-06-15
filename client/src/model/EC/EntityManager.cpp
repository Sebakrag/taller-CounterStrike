#include "../../../../client/include/model/EC/EntityManager.h"

#include <stdexcept>

EntityManager::EntityManager(ComponentManager& cm, const int numPlayers):
        entt_factory(cm, numPlayers) {}

Entity EntityManager::create_entity(const EntitySnapshot& snap) {
    Entity new_entt;
    if (!free_ids.empty()) {
        const auto it = free_ids.begin();
        new_entt = *it;
        free_ids.erase(it);
    } else if (next_id <= MAX_ENTITIES) {
        new_entt = next_id++;
    } else {
        throw std::runtime_error("Cannot create entity because the game reached the MAX_ENTITIES");
    }

    server_entt_id_to_entity[snap.server_entt_id] = new_entt;
    entt_factory.create_specific_entity(new_entt, snap);
    return new_entt;
}

Entity EntityManager::create_local_player(const LocalPlayerInfo& localPlayerInfo) {
    Entity new_entt;
    if (!free_ids.empty()) {
        const auto it = free_ids.begin();
        new_entt = *it;
        free_ids.erase(it);
    } else if (next_id <= MAX_ENTITIES) {
        new_entt = next_id++;
    } else {
        throw std::runtime_error("Cannot create entity because the game reached the MAX_ENTITIES");
    }

    server_entt_id_to_entity[localPlayerInfo.server_entt_id] = new_entt;
    entt_factory.createEntityPlayer(new_entt, localPlayerInfo);
    return new_entt;
}


void EntityManager::destroy_entity_immediately(const ServerEntityID& serv_entt_id) {
    auto it = server_entt_id_to_entity.find(serv_entt_id);
    if (it != server_entt_id_to_entity.end()) {
        entt_factory.destroy(it->second);  // libera todos sus componentes.
        free_ids.insert(it->second);       // Dado que es un set garantiza la unicidad.
        server_entt_id_to_entity.erase(it);
    }
}

Entity EntityManager::get(const ServerEntityID& serv_entt_id) const {
    auto it = server_entt_id_to_entity.find(serv_entt_id);
    if (it == server_entt_id_to_entity.end()) {
        return INVALID_ENTITY;
    }

    return it->second;
}
