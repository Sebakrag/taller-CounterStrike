#ifndef GAME_INFO_H_
#define GAME_INFO_H_

#include <cstdint>
#include <string>
#include <vector>

#include "../types.h"

#include "bullet_info.h"
#include "item_info.h"
#include "player_info.h"

// Acá están todos los datos que va a recibir la interfaz grafica
// en cada frame. Son los que enviará el gameloop del server en cada iteración.

class GameInfo {
public:
    GamePhase gamePhase;
    bool bombPlanted;
    int bombX;
    int bombY;
    double timeLeft;

    // Podriamos reemplazar estos 3 vectores por un unico vector de la sig forma:
    // std::vector<EntitySnapshot> entities;
    std::vector<PlayerInfo> players;
    std::vector<BulletInfo> bullets;
    std::vector<ItemInfo> items;

    GameInfo();

    explicit GameInfo(GamePhase gamePhase, double timeLeft, const std::vector<PlayerInfo>& players);

    explicit GameInfo(GamePhase gamePhase, bool bombPlanted, int bombX, int bombY, double timeLeft,
                      const std::vector<PlayerInfo>& players,
                      const std::vector<BulletInfo>& bullets, const std::vector<ItemInfo>& items);

    GameInfo(const GameInfo& other);
    GameInfo& operator=(const GameInfo& other);

    explicit GameInfo(const std::vector<uint8_t>& bytes);

    std::vector<uint8_t> toBytes() const;

    void print();
};

#endif  // GAME_INFO_H_
