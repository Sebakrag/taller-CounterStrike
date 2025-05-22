#ifndef GAME_INFO_H_
#define GAME_INFO_H_

#include <string>
#include <vector>

#include "types.h"

// Acá están todos los datos que va a recibir la interfaz grafica
// en cada frame. Son los qque enviará el gameloop del server en cada iteración.

struct PlayerInfo {
    std::string username;  // id
    int health;
    int money;
    int pos_x;
    int pos_y;
    Vector2 direction;  // a dónde está mirando el player
    TypeWeapon weapon_selected;
    int ammo_weapon;
    // estado {quieto, caminando, atacando, recibiendo daño, muriendo, agarrando, etc}
    // skin ?
    // team ?
};

// faltan agregar cosas.
class GameInfo {
public:
    GamePhase gamePhase;
    std::vector<PlayerInfo> players;

    explicit GameInfo(GamePhase gamePhase, const std::vector<PlayerInfo>& players);

    GameInfo(const GameInfo&);
    // GameInfo(const GameInfo&) = default;


    GameInfo& operator=(const GameInfo&);
};

#endif  // GAME_INFO_H_
