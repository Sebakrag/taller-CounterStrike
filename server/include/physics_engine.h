#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include <cmath>
#include <memory>
#include <random>
#include <vector>

#include "map.h"
#include "player.h"

#define RADIUS_PLAYER_TOUCH 20.0f  // distancia de contacto para agarrar un drop

class PhysicsEngine {
public:
    static void movePlayer(Player& player, float dirX, float dirY, float deltaTime, Map& map);
    static bool shotHitPlayer(float projX, float projY, const Player& shooter, const Player& target, const FireWeapon& weapon, float& impactDistance, bool& hitByPrecision);
    static float calculatePrecisionByDistance(float distance, float maxRange, float basePrecision);
    static bool knifeHit(float originX, float originY, float dirX, float dirY, const Player& target,
                         float& outDist);
    static bool playerTouchingItem(float playerPosX, float playerPosY, float itemPosX,
                                   float itemPosY);
};

#endif
