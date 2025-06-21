#ifndef GAME_INFO_H_
#define GAME_INFO_H_

#include <cstdint>
#include <vector>

#include "../dtos/EntitySnapshot.h"
#include "../types.h"

#include "bullet_info.h"
#include "local_player_info.h"
#include "player_info.h"
#include "weapon_info.h"
#include "shop_info.h"

// Acá están todos los datos que va a recibir la interfaz grafica
// en cada frame. Son los que enviará el gameloop del server en cada iteración.

class GameInfo {
private:
    std::vector<EntitySnapshot> entities;

public:
    GamePhase gamePhase;
    bool bombPlanted;
    int bombX;
    int bombY;
    float timeLeft;  // En fase de preparación, indica los segundos que restan para comenzar el
                     // juego. En el juego, indica cuanto falta para que explote la bomba, luego de
                     // ser colocada.

    LocalPlayerInfo localPlayer;
    std::vector<PlayerInfo> otherPlayers;
    std::vector<BulletInfo> bullets;
    std::vector<WeaponInfo> weapons;
    ShopInfo shop;

    GameInfo() {}

    GameInfo(GamePhase gamePhase, bool bombPlanted, int bombX, int bombY, float timeLeft,
             const LocalPlayerInfo& localPlayer, const std::vector<PlayerInfo>& otherPlayers,
             const std::vector<BulletInfo>& bullets, const std::vector<WeaponInfo>& items, const ShopInfo& shop);

    GameInfo(const GameInfo& other) = default;
    GameInfo& operator=(const GameInfo& other) = default;

    // private:
    explicit GameInfo(const std::vector<uint8_t>& bytes);

    std::vector<uint8_t> toBytes() const;

    std::vector<EntitySnapshot> getSnapshots() const;

    void print() const;
};

#endif  // GAME_INFO_H_
