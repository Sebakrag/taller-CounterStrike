#ifndef ENTITY_SNAPSHOT_T_H
#define ENTITY_SNAPSHOT_T_H

#include <cstdint>
#include <type_traits>
#include <variant>

#include "client/include/model/utils/SpriteType.h"
#include "common/types.h"
#include "common/utils/EntityType.h"

struct PlayerSnapshot;
struct WeaponSnapshot;

using ServerEntityID = uint32_t;
using SpecificData = std::variant<std::monostate, PlayerSnapshot, WeaponSnapshot>;

struct PlayerSnapshot {
    int hp;  // health
    int money;
    int ammo;
    PlayerState state;
    ServerEntityID equipped_weapon_id;
    Team team;
};

struct WeaponSnapshot {
    WeaponState state;
};

struct EntitySnapshot {
    ServerEntityID server_entt_id;
    EntityType type;
    SpriteType sprite_type;
    float pos_x;
    float pos_y;
    float angle;
    bool is_alive;  // Indica si la entidad sigue viva o hay que eliminarla.

    SpecificData data;

    // TODO: conviene distinguir entre el local player y los demas players?
    ///
    /// @brief Constructor para un PLAYER.
    ///
    EntitySnapshot(ServerEntityID id, EntityType entt_type, SpriteType sprite, float pos_x,
                   float pos_y, float angle, bool alive, int hp, int money, int ammo,
                   PlayerState state, ServerEntityID equipped_weapon_id, Team team);

    ///
    /// @brief Constructor para un WEAPON.
    ///
    EntitySnapshot(ServerEntityID id, EntityType entt_type, SpriteType sprite, float pos_x,
                   float pos_y, float angle, bool alive, WeaponState state);

    ///
    /// @brief Constructor para un BULLET.
    ///
    EntitySnapshot(ServerEntityID id, EntityType entt_type, SpriteType sprite, float pos_x,
                   float pos_y, float angle, bool alive);

    EntitySnapshot(const EntitySnapshot& other) = default;
    EntitySnapshot& operator=(const EntitySnapshot& other) = default;
};


// struct EntitySnapshot {
//     ServerEntityID server_entt_id;
//     float pos_x, pos_y;
//     float angle;  // represents the orientation where the player is looking.
//     SpriteType sprite_type;
//     EntityType entt_type;
//     int hp;  // health
//     float money;
//     int ammo;
//     bool is_alive;  // is alive or dead? (this could be useful for any entity, not just players)
//     Team team;
//     PlayerState player_state;
//     ServerEntityID equipped_weapon_id;
//     WeaponState weapon_state;
//
//     // TODO: Eliminar este constructor. Lo creo para poder crear el Client para probar el
//     // renderizado. Lo inicializo con valores random.
//     EntitySnapshot():
//             server_entt_id(0),
//             pos_x(0),
//             pos_y(0),
//             angle(0),
//             sprite_type(SpriteType::ARTIC_AVENGER),
//             entt_type(EntityType::ANTI_TERRORIST),
//             hp(100),
//             money(500),
//             is_alive(true),
//             team(Team::CounterTerrorist),
//             player_state(PlayerState::Idle) {}
//
//     // Constructor por copia
//     EntitySnapshot(const EntitySnapshot& other) = default;
//
//     // Operador de asignación
//     EntitySnapshot& operator=(const EntitySnapshot& other) = default;
//
//     // para un item (drop).
//     explicit EntitySnapshot(const ServerEntityID server_entt_id, const float pos_x,
//                             const float pos_y, const SpriteType sprite_type,
//                             const EntityType entt_type);
//
//     // para un item (drop).
//     EntitySnapshot(const ServerEntityID server_entt_id, const float pos_x, const float pos_y,
//                    const SpriteType sprite_type, const EntityType entt_type,
//                    const WeaponState weapon_state);
//
//     // para una bullet.
//     explicit EntitySnapshot(const ServerEntityID server_entt_id, const float pos_x,
//                             const float pos_y, const float angle, const SpriteType sprite_type,
//                             const EntityType entt_type);
//
//     // para un player.
//     explicit EntitySnapshot(const ServerEntityID server_entt_id, const float pos_x,
//                             const float pos_y, const float angle, const SpriteType sprite_type,
//                             const EntityType entt_type, const int hp, const float money, Team
//                             team, PlayerState state, bool is_alive = true):
//             server_entt_id(server_entt_id),
//             pos_x(pos_x),
//             pos_y(pos_y),
//             angle(angle),
//             sprite_type(sprite_type),
//             entt_type(entt_type),
//             hp(hp),
//             money(money),
//             is_alive(is_alive),
//             team(team),
//             player_state(state) {}
//
//     // ELIMINARLO cuando ya no se use en logica de la interfaz
//     EntitySnapshot(const ServerEntityID server_entt_id, const float pos_x, const float pos_y,
//                    const float angle, const SpriteType sprite_type, const EntityType entt_type,
//                    const int hp, const float money, const bool is_alive):
//             server_entt_id(server_entt_id),
//             pos_x(pos_x),
//             pos_y(pos_y),
//             angle(angle),
//             sprite_type(sprite_type),
//             entt_type(entt_type),
//             hp(hp),
//             money(money),
//             is_alive(is_alive) {}
//     // explicit EntitySnapshot(const std::vector<uint8_t>& bytes);
//     void print();  // para testear
//     // std::vector<uint8_t> toBytes() const;
// };

static_assert(std::is_trivially_destructible_v<EntitySnapshot>,
              "EntitySnapshot debe ser trivially destructible in order to keep clear() O(1).");

#endif  // ENTITY_SNAPSHOT_T_H
