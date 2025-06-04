#ifndef TYPES_H_
#define TYPES_H_

#include <string>
#include <vector>

#include "utils/Vec2D.h"

enum class Weapon { None, Glock, Ak47, M3, Awp };
enum class TypeWeapon { Primary, Secondary, Knife, Bomb };
enum class Team { Terrorist, CounterTerrorist };
enum class GamePhase { Preparation, Combat, EndOfMatch };

enum class PlayerState : unsigned char { Idle, Walking, Attacking, TakingDamage, PickingUp, Dead };
enum class WeaponState : unsigned char { DROPPED = 1, EQUIPPED, HIDDEN };

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

struct PlayerInfoLobby {
    std::string username;
    Team team;

    PlayerInfoLobby() {}
    PlayerInfoLobby(const std::string& username, const Team team): username(username), team(team) {}
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
enum GameActionType { Null, BuyWeapon, BuyAmmo, Attack, Walk, ChangeWeapon, PickUp, Rotate };

struct GameAction {
    GameActionType type = Null;
    Weapon weapon = Weapon::None;               // rellenar si se quiere comprar una.
    TypeWeapon typeWeapon = TypeWeapon::Knife;  // rellenar si se quiere cambiar o comprar municion.
    int count_ammo = 0;                         // rellenar si quiere comprar municion
    Vec2D direction;

    GameAction() {}
    explicit GameAction(GameActionType type, Weapon weapon = Weapon::Glock):
            type(type), weapon(weapon) {}

    GameAction(GameActionType type, TypeWeapon typeWeapon, int count_ammo = 0):
            type(type), typeWeapon(typeWeapon), count_ammo(count_ammo) {}

    GameAction(GameActionType type, const Vec2D& direction): type(type), direction(direction) {}

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
