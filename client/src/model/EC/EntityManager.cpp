#include "client/include/model/EC/EntityManager.h"

Entity EntityManager::create_entity(const ServerEntityID& serv_entt_id) {
    Entity new_entt;
    if (!free_ids.empty()) {
        const auto it = free_ids.begin();
        new_entt = *it;
        free_ids.erase(it);
    } else {
        new_entt = next_id++;
    }

    server_entt_id_to_entity[serv_entt_id] = new_entt;
    return new_entt;
}

void EntityManager::destroy_entity_immediately(const ServerEntityID& serv_entt_id) {
    auto it = server_entt_id_to_entity.find(serv_entt_id);
    if (it != server_entt_id_to_entity.end()) {
        // Si elimino una entidad entonces deberia eliminar todos sus componentes asociados.
        // QUizas que EntityManager deberia tener una referencia a ComponentManager.
        // cm.remove_all_components_of(it->second);
        free_ids.insert(it->second);  // Dado que es un set garantiza la unicidad.
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
