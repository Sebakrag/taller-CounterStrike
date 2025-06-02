#ifndef ENTITY_SNAPSHOT_T_H
#define ENTITY_SNAPSHOT_T_H

#include <cstdint>
#include <type_traits>
#include <vector>

#include "../../client/include/model/utils/SpriteType.h"
#include "../types.h"
#include "../utils/EntityType.h"
using ServerEntityID = uint32_t;

struct EntitySnapshot {
    ServerEntityID server_entt_id;
    float pos_x, pos_y;
    float angle;  // represents the orientation where the player is looking.
    SpriteType sprite_type;
    EntityType entt_type;
    int hp;  // health
    float money;
    int ammo;
    bool is_alive;  // is alive or dead? (this could be useful for any entity, not just players)
    Team team;
    PlayerState player_state;
    ServerEntityID equipped_weapon_id;

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
            is_alive(true),
            team(Team::CounterTerrorist),
            player_state(PlayerState::Idle) {}

    // Constructor por copia
    EntitySnapshot(const EntitySnapshot& other):
            server_entt_id(other.server_entt_id),
            pos_x(other.pos_x),
            pos_y(other.pos_y),
            angle(other.angle),
            sprite_type(other.sprite_type),
            entt_type(other.entt_type),
            hp(other.hp),
            money(other.money),
            ammo(other.ammo),
            is_alive(other.is_alive),
            team(other.team),
            player_state(other.player_state) {}

    // Operador de asignación
    EntitySnapshot& operator=(const EntitySnapshot& other) {
        if (this != &other) {
            server_entt_id = other.server_entt_id;
            pos_x = other.pos_x;
            pos_y = other.pos_y;
            angle = other.angle;
            sprite_type = other.sprite_type;
            entt_type = other.entt_type;
            hp = other.hp;
            money = other.money;
            ammo = other.ammo;
            is_alive = other.is_alive;
            team = other.team;
            player_state = other.player_state;
        }
        return *this;
    }

    // para un item (drop).
    explicit EntitySnapshot(const ServerEntityID server_entt_id, const float pos_x,
                            const float pos_y, const SpriteType sprite_type,
                            const EntityType entt_type);

    // para una bullet.
    explicit EntitySnapshot(const ServerEntityID server_entt_id, const float pos_x,
                            const float pos_y, const float angle, const SpriteType sprite_type,
                            const EntityType entt_type);

    // para un player.
    explicit EntitySnapshot(const ServerEntityID server_entt_id, const float pos_x,
                            const float pos_y, const float angle, const SpriteType sprite_type,
                            const EntityType entt_type, const int hp, const float money, Team team,
                            PlayerState state, bool is_alive = true):
            server_entt_id(server_entt_id),
            pos_x(pos_x),
            pos_y(pos_y),
            angle(angle),
            sprite_type(sprite_type),
            entt_type(entt_type),
            hp(hp),
            money(money),
            is_alive(is_alive),
            team(team),
            player_state(state) {}

    // ELIMINARLO cuando ya no se use en logica de la interfaz
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
    // explicit EntitySnapshot(const std::vector<uint8_t>& bytes);
    void print();  // para testear
    // std::vector<uint8_t> toBytes() const;
};

static_assert(std::is_trivially_destructible_v<EntitySnapshot>,
              "EntitySnapshot debe ser trivially destructible in order to keep clear() O(1).");

#endif  // ENTITY_SNAPSHOT_T_H
