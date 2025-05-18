#ifndef GAME_INFO_H_
#define GAME_INFO_H_

#include <string>
#include <vector>

#include "types.h"

// Acá están todos los data que va a recibir la interfaz grafica
// en cada frame.

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
    const GamePhase gamePhase;
    const std::vector<PlayerInfo> players;

    explicit GameInfo(GamePhase gamePhase, const std::vector<PlayerInfo>& players);
};

#endif  // GAME_INFO_H_
