#ifndef ENTITY_SNAPSHOT_T_H
#define ENTITY_SNAPSHOT_T_H

#include <cstdint>
#include <type_traits>

#include "client/include/model/EC/EntityType.h"
#include "client/include/model/utils/SpriteType.h"

using ServerEntityID = uint32_t;

struct EntitySnapshot {
    const ServerEntityID server_entt_id;
    const float pos_x, pos_y;
    const float angle;  // represents the orientation where the player is looking.
    const SpriteType sprite_type;
    const EntityType entt_type;
    const int hp;  // health
    const float money;
    const bool
            is_alive;  // is alive or dead? (this could be useful for any entity, not just players)

    // TODO: Eliminar este constructor. Lo creo para poder crear el Client para probar el
    // renderizado. Lo inicializo con valores random.
    EntitySnapshot():
            server_entt_id(0),
            pos_x(0),
            pos_y(0),
            angle(0),
            sprite_type(SpriteType::ARTIC_AVENGER),
            entt_type(EntityType::ANTI_TERRORIST),
            hp(100),
            money(500),
            is_alive(true) {}

    EntitySnapshot(const ServerEntityID server_entt_id, const float pos_x, const float pos_y,
                   const float angle, const SpriteType sprite_type, const EntityType entt_type,
                   const int hp, const float money, const bool is_alive):
            server_entt_id(server_entt_id),
            pos_x(pos_x),
            pos_y(pos_y),
            angle(angle),
            sprite_type(sprite_type),
            entt_type(entt_type),
            hp(hp),
            money(money),
            is_alive(is_alive) {}
};

static_assert(std::is_trivially_destructible_v<EntitySnapshot>,
              "EntitySnapshot debe ser trivially destructible in order to keep clear() O(1).");

#endif  // ENTITY_SNAPSHOT_T_H
