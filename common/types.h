#ifndef TYPES_H_
#define TYPES_H_

#include <string>
#include <vector>

enum class Weapon { None, Glock, Ak47, M3, Awp };
enum class TypeWeapon { Primary, Secondary, Knife, Bomb };
enum class Team { Terrorist, CounterTerrorist };
enum class GamePhase { Preparation, Combat, EndOfMatch };

struct Vector2 {
    float x;
    float y;

    Vector2(): x(0), y(0) {}
    explicit Vector2(float x, float y): x(x), y(y) {}

    Vector2(const Vector2&) = default;
    Vector2& operator=(const Vector2&) = default;
};

struct PlayerInfoLobby {
    std::string username;
    Team team;

    PlayerInfoLobby() {}
    explicit PlayerInfoLobby(std::string username, Team team): username(username), team(team) {}
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
enum GameActionType { Null, BuyWeapon, BuyAmmo, Attack, Walk, ChangeWeapon, PickUp };

struct GameAction {
    GameActionType type = Null;
    Weapon weapon = Weapon::None;               // rellenar si se quiere comprar una.
    TypeWeapon typeWeapon = TypeWeapon::Knife;  // rellenar si se quiere cambiar o comprar municion.
    int count_ammo = 0;                         // rellenar si quiere comprar municion
    Vector2 direction;

    GameAction() {}
    explicit GameAction(GameActionType type, Weapon weapon = Weapon::Glock):
            type(type), weapon(weapon) {}

    explicit GameAction(GameActionType type, TypeWeapon typeWeapon, int count_ammo = 0):
            type(type), typeWeapon(typeWeapon), count_ammo(count_ammo) {}

    explicit GameAction(GameActionType type, Vector2 direction): type(type), direction(direction) {}

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
