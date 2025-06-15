#include "EntitySnapshot.h"

#include <stdexcept>

EntitySnapshot::EntitySnapshot(const ServerEntityID id, const EntityType entt_type,
                               const SpriteType sprite, const float pos_x, const float pos_y,
                               const float angle, const bool alive, const int hp, const int money,
                               const int ammo, const PlayerState state,
                               const ServerEntityID equipped_weapon_id, const Team team):
        server_entt_id(id),
        type(entt_type),
        sprite_type(sprite),
        pos_x(pos_x),
        pos_y(pos_y),
        angle(angle),
        is_alive(alive),
        data(PlayerSnapshot{hp, money, ammo, state, equipped_weapon_id, team}) {
    if (entt_type != EntityType::PLAYER) {
        throw std::runtime_error(
                "Fallo en el constructor de EntitySnapshot(). Se requiere EntityType::PLAYER");
    }
}
EntitySnapshot::EntitySnapshot(ServerEntityID id, EntityType entt_type, SpriteType sprite,
                               float pos_x, float pos_y, float angle, bool alive, PlayerState state,
                               ServerEntityID equipped_weapon_id, Team team):
        server_entt_id(id),
        type(entt_type),
        sprite_type(sprite),
        pos_x(pos_x),
        pos_y(pos_y),
        angle(angle),
        is_alive(alive),
        // TODO: INFORMACIÓN HARDCODEADA.
        data(PlayerSnapshot{0, 0, 0, state, equipped_weapon_id, team}) {}

EntitySnapshot::EntitySnapshot(const ServerEntityID id, const EntityType entt_type,
                               const SpriteType sprite, const float pos_x, const float pos_y,
                               const float angle, const bool alive, const WeaponState state):
        server_entt_id(id),
        type(entt_type),
        sprite_type(sprite),
        pos_x(pos_x),
        pos_y(pos_y),
        angle(angle),
        is_alive(alive),
        data(WeaponSnapshot{state}) {
    if (entt_type != EntityType::WEAPON) {
        throw std::runtime_error(
                "Fallo en el constructor de EntitySnapshot(). Se requiere EntityType::WEAPON");
    }
}

EntitySnapshot::EntitySnapshot(const ServerEntityID id, const EntityType entt_type,
                               const SpriteType sprite, const float pos_x, const float pos_y,
                               const float angle, const bool alive):
        server_entt_id(id),
        type(entt_type),
        sprite_type(sprite),
        pos_x(pos_x),
        pos_y(pos_y),
        angle(angle),
        is_alive(alive) {
    if (entt_type != EntityType::BULLET) {
        throw std::runtime_error(
                "Fallo en el constructor de EntitySnapshot(). Se requiere EntityType::BULLET");
    }
}
