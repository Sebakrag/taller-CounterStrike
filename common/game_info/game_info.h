#ifndef GAME_INFO_H_
#define GAME_INFO_H_

#include <cstdint>
#include <string>
#include <vector>

#include "../types.h"

#include "player_info.h"

// Acá están todos los datos que va a recibir la interfaz grafica
// en cada frame. Son los que enviará el gameloop del server en cada iteración.

struct BulletInfo {
    int pos_x;
    int pos_y;
    Vector2 direction;
};
struct ItemsInfo {  // drops (objetos recolectables)
    int pos_x;
    int pos_y;
};

class GameInfo {
public:
    GamePhase gamePhase;
    bool bombPlanted;
    int bombX;
    int bombY;
    double timeLeft;

    std::vector<PlayerInfo> players;
    std::vector<BulletInfo> bullets;
    std::vector<ItemsInfo> items;

    GameInfo();

    explicit GameInfo(GamePhase gamePhase, double timeLeft, const std::vector<PlayerInfo>& players);

    explicit GameInfo(GamePhase gamePhase, bool bombPlanted, int bombX, int bombY, double timeLeft,
                      const std::vector<PlayerInfo>& players,
                      const std::vector<BulletInfo>& bullets, const std::vector<ItemsInfo>& items);

    // Constructor de copia
    GameInfo(const GameInfo& other);

    // Operador de asignación
    GameInfo& operator=(const GameInfo& other);

    std::vector<uint8_t> toBytes() const;
};

#endif  // GAME_INFO_H_
