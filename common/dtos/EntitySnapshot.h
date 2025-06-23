#ifndef ENTITY_SNAPSHOT_T_H
#define ENTITY_SNAPSHOT_T_H

#include <cstdint>
#include <type_traits>
#include <variant>

#include "../../client/include/model/utils/SpriteType.h"
#include "../../common/types.h"
#include "../../common/utils/EntityType.h"

struct PlayerSnapshot;
struct WeaponSnapshot;
struct BombSnapshot;

using ServerEntityID = uint32_t;
using SpecificData = std::variant<std::monostate, PlayerSnapshot, WeaponSnapshot, BombSnapshot>;

struct PlayerSnapshot {
    int hp;  // health
    int money;
    PlayerState state;
    ServerEntityID equipped_weapon_id;
    Weapon equipped_weapon;
    Team team;
    TypeWeapon weapon_type;
};

struct WeaponSnapshot {
    WeaponState state;
    // AmmoInfo ammoInfo;
};

struct BombSnapshot {
    BombState state;
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

    ///
    /// @brief Constructor para el LOCAL PLAYER.
    ///
    EntitySnapshot(ServerEntityID id, EntityType entt_type, SpriteType sprite, float pos_x,
                   float pos_y, float angle, bool alive, int hp, int money, PlayerState state,
                   ServerEntityID equipped_weapon_id, Weapon equppedWeapon, Team team,
                   TypeWeapon weapon_type);

    ///
    /// @brief Constructor para el PLAYER.
    ///
    EntitySnapshot(ServerEntityID id, EntityType entt_type, SpriteType sprite, float pos_x,
                   float pos_y, float angle, bool alive, PlayerState state,
                   ServerEntityID equipped_weapon_id, Weapon equippedWeapon, Team team,
                   TypeWeapon weapon_type);

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

    ///
    /// @brief Constructor para un BOMB.
    ///
    EntitySnapshot(ServerEntityID id, EntityType entt_type, SpriteType sprite, float pos_x,
                   float pos_y, float angle, bool alive, BombState state);

    EntitySnapshot(const EntitySnapshot& other) = default;
    EntitySnapshot& operator=(const EntitySnapshot& other) = default;
};

static_assert(std::is_trivially_destructible_v<EntitySnapshot>,
              "EntitySnapshot debe ser trivially destructible in order to keep clear() O(1).");

#endif  // ENTITY_SNAPSHOT_T_H
