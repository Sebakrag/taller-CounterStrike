#ifndef BOMB_H
#define BOMB_H

#include <cstdint>

#include "server/include/map.h"
#include "common/types.h"
#include "server/include/id_generator.h"

class Bomb {
private:
    BombState state;
    float posX, posY;
    Vec2D plantedPosition;
    std::string carrierId;
    double timer;
    const double TIME_TO_EXPLODE;
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

    // Update loop
    void update(double elapsedTime);

    // Accesos
    float getX() const;
    float getY() const;
    Vec2D getPlantedPosition() const;
    std::string getCarrierId() const;
    BombState getState() const;

};

#endif
