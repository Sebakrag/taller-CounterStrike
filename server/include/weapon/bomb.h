#ifndef BOMB_H
#define BOMB_H

#include <cstdint>
#include <string>

#include "../../../common/game_info/game_info.h"
#include "../../../common/types.h"
#include "../../../server/include/id_generator.h"
#include "../../../server/include/map.h"

class Bomb {
private:
    BombState state;
    float posX, posY;
    Vec2D plantedPosition;
    std::string carrierId;
    double timer;
    const double timeToExplode;
    uint32_t serverId;

public:
    Bomb();

    void assignTo(const std::string& playerId);
    void reset();

    // Verificacion de estado
    bool isCarriedBy(const std::string& playerId) const;
    bool hasExploded() const;
    bool isDefused() const;
    bool isPlanted() const;
    bool isDropped() const;
    bool isCarried() const;

    // Interacciones
    void drop(float x, float y);
    void pickUp(const std::string& playerId);
    bool plant(float x, float y, Map& map);
    bool defuse();
    void equip();
    void hide();

    // Update loop
    void update(double elapsedTime);

    // Accesos
    uint32_t getServerId() const;
    float getX() const;
    float getY() const;
    Vec2D getPlantedPosition() const;
    std::string getCarrierId() const;
    BombState getState() const;
    double getTimer() const;

    BombInfo generateBombInfo() const;

    // CONSTANTES static para inicializar en server.cpp.
private:
    static bool initialized;  // para asegurar que se llame init una unica vez.
    static double TIME_TO_EXPLODE;

public:
    // Para cargar los valores del config.yaml
    static void init(double time_to_explode);
};

#endif
