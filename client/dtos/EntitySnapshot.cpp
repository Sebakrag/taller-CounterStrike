#include "EntitySnapshot.h"

EntitySnapshot::EntitySnapshot(const ServerEntityID server_entt_id, const float pos_x,
                               const float pos_y, const float angle, const SpriteType sprite_type,
                               const int hp, const float money):
        server_entt_id(server_entt_id),
        pos_x(pos_x),
        pos_y(pos_y),
        angle(angle),
        sprite_type(sprite_type),
        hp(hp),
        money(money) {}
