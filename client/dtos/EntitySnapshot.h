#ifndef ENTITY_SNAPSHOT_T_H
#define ENTITY_SNAPSHOT_T_H

#include <cstdint>
#include <type_traits>

#include "client/include/model/utils/SpriteType.h"

using ServerEntityID = uint32_t;

struct EntitySnapshot {
    const ServerEntityID server_entt_id;
    const float pos_x, pos_y;
    const float angle;  // represents the orientation where the player is looking.
    const SpriteType sprite_type;
    // const EntityType entt_type; // could be: player, bullet, weapon, etc...
    const int hp;  // health
    const float money;
    const bool
            is_alive;  // is alive or dead? (this could be useful for any entity, not just players)

    EntitySnapshot(ServerEntityID server_entt_id, float pos_x, float pos_y, float angle,
                   SpriteType sprite_type, int hp, float money);
};

static_assert(std::is_trivially_destructible_v<EntitySnapshot>,
              "EntitySnapshot debe ser trivially destructible in order to keep clear() O(1).");

#endif  // ENTITY_SNAPSHOT_T_H
