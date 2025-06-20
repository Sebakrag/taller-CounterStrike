#ifndef TYPES_H_
#define TYPES_H_

#include <cstdint>
#include <string>
#include <vector>

#include "utils/Vec2D.h"


using ServerEntityID = uint32_t;

enum class Weapon : unsigned char { None, Glock, Ak47, M3, Awp, Knife, Bomb };
enum class TypeWeapon : unsigned char { Primary, Secondary, Knife, Bomb };
enum class Team : unsigned char { Terrorist, CounterTerrorist };
enum class GamePhase : unsigned char { Preparation, Combat, EndOfMatch };
enum class BombState : unsigned char { Dropped, Carried, Planted, Exploded, Defused };

enum class PlayerState : unsigned char { Idle, Walking, Attacking, TakingDamage, PickingUp, Dead };
enum class WeaponState : unsigned char { DROPPED, EQUIPPED, HIDDEN };

enum class PlayerSkin {
    Terrorist1,
    Terrorist2,
    Terrorist3,
    Terrorist4,
    CounterTerrorist1,
    CounterTerrorist2,
    CounterTerrorist3,
    CounterTerrorist4
};

enum class TypeItem { Coin, Glock, Ak47, M3, Awp, Bomb };

enum class TypeTileMap { Desert, Aztec, Training };

struct PlayerInfoLobby {
    std::string username;
    Team team;
    bool is_player_host;  // puede variar si el creador original abandona la partida

    PlayerInfoLobby() {}
    PlayerInfoLobby(const std::string& username, const Team team, bool is_player_host):
            username(username), team(team), is_player_host(is_player_host) {}
};

struct MatchRoomInfo {
    bool matchStarted;
    std::vector<PlayerInfoLobby> players;

    MatchRoomInfo() {}
    explicit MatchRoomInfo(const std::vector<PlayerInfoLobby>& players, bool matchStarted = false):
            matchStarted(matchStarted), players(players) {}
};

// DTOs
//--------
// menu.
enum MenuActionType { Exit, Create, Join, List };

struct MenuAction {
    const MenuActionType type;
    const std::string name_match;
    const int id_scenary = 0;

    explicit MenuAction(MenuActionType type, const std::string& name_match = "",
                        int id_scenary = 0):
            type(type), name_match(name_match), id_scenary(id_scenary) {}
};

// lobby
enum class LobbyAction { QuitMatch, StartMatch, ListPlayers };

// game
enum GameActionType {
    Null,
    BuyWeapon,
    BuyAmmo,
    Attack,
    Walk,
    ChangeWeapon,
    PickUp,
    Rotate,
    PlantBomb,
    DefuseBomb
};

struct GameAction {
    GameActionType type = Null;
    Weapon weapon = Weapon::None;               // rellenar si se quiere comprar una.
    TypeWeapon typeWeapon = TypeWeapon::Knife;  // rellenar si se quiere cambiar o comprar municion.
    int count_ammo = 0;                         // rellenar si quiere comprar municion
    Vec2D direction;
    float angle = 0;  // para Rotate

    GameAction() {}
    explicit GameAction(GameActionType type, Weapon weapon = Weapon::Glock):
            type(type), weapon(weapon) {}

    explicit GameAction(GameActionType type, TypeWeapon typeWeapon, int count_ammo = 0):
            type(type), typeWeapon(typeWeapon), count_ammo(count_ammo) {}

    explicit GameAction(GameActionType type, const Vec2D& direction):
            type(type), direction(direction) {}

    explicit GameAction(GameActionType type, float angle): type(type), angle(angle) {}

    GameAction(const GameAction&) = default;
    GameAction& operator=(const GameAction&) = default;
};

struct PlayerAction {
    std::string player_username;
    GameAction gameAction;

    PlayerAction() {}
    explicit PlayerAction(const std::string& player_username, const GameAction& gameAction):
            player_username(player_username), gameAction(gameAction) {}
};

#endif  // TYPES_H_
