#ifndef GAME_INFO_H_
#define GAME_INFO_H_

#include <cstdint>
#include <string>
#include <vector>

#include "../dtos/EntitySnapshot.h"
#include "../types.h"

#include "bullet_info.h"
#include "item_info.h"
#include "player_info.h"
#include "projectile_info.h"

// Acá están todos los datos que va a recibir la interfaz grafica
// en cada frame. Son los que enviará el gameloop del server en cada iteración.

class GameInfo {
private:
    std::vector<EntitySnapshot>
            entities;  // TODO. Aplicar logica de generar el vector en cada constructor.

public:
    GamePhase gamePhase;
    bool bombPlanted;
    int bombX;
    int bombY;
    float timeLeft;  // En fase de preparación, indica los segundos que restan para comenzar el
                     // juego. En el juego, indica cuanto falta para que explote la bomba, luego de
                     // ser colocada.

    std::vector<PlayerInfo> players;
    std::vector<BulletInfo> bullets;
    std::vector<ItemInfo> items;
    std::vector<ProjectileInfo> projectiles;

    GameInfo() {}

    explicit GameInfo(GamePhase gamePhase, float timeLeft, const std::vector<PlayerInfo>& players, const std::vector<ProjectileInfo>& projectiles);

    explicit GameInfo(GamePhase gamePhase, bool bombPlanted, int bombX, int bombY, float timeLeft,
                      const std::vector<PlayerInfo>& players,
                      const std::vector<BulletInfo>& bullets, const std::vector<ItemInfo>& items);


    GameInfo(const GameInfo& other);
    GameInfo& operator=(const GameInfo& other);

    explicit GameInfo(const std::vector<uint8_t>& bytes);

    std::vector<uint8_t> toBytes() const;

    // nota. por ahora solo funciona si se usa el constructor con el buffer.
    std::vector<EntitySnapshot> getSnapshots();

    void print() const;
};

#endif  // GAME_INFO_H_
