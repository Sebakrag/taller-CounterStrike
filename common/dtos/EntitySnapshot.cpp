#include "EntitySnapshot.h"

#include <stdexcept>


// para un item (drop).
EntitySnapshot::EntitySnapshot(const ServerEntityID server_entt_id, const float pos_x,
                               const float pos_y, const SpriteType sprite_type,
                               const EntityType entt_type):
        server_entt_id(server_entt_id),
        pos_x(pos_x),
        pos_y(pos_y),
        sprite_type(sprite_type),
        entt_type(entt_type) {
    if (entt_type != EntityType::DROP) {
        throw std::runtime_error(
                "Fallo en el constructor de EntitySnapshot(). Se requiere EntityType::DROP");
    }
}

EntitySnapshot::EntitySnapshot(const ServerEntityID server_entt_id, const float pos_x,
                               const float pos_y, const SpriteType sprite_type,
                               const EntityType entt_type, const WeaponState weapon_state):
        server_entt_id(server_entt_id),
        pos_x(pos_x),
        pos_y(pos_y),
        sprite_type(sprite_type),
        entt_type(entt_type),
        is_alive(true),
        weapon_state(weapon_state) {}

// para una bullet.
EntitySnapshot::EntitySnapshot(const ServerEntityID server_entt_id, const float pos_x,
                               const float pos_y, const float angle, const SpriteType sprite_type,
                               const EntityType entt_type):
        server_entt_id(server_entt_id),
        pos_x(pos_x),
        pos_y(pos_y),
        angle(angle),
        sprite_type(sprite_type),
        entt_type(entt_type) {
    if (entt_type != EntityType::BULLET) {
        throw std::runtime_error(
                "Fallo en el constructor de EntitySnapshot(). Se requiere EntityType::BULLET");
    }
}
// mas constructores
