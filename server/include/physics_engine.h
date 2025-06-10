#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include <cmath>
#include <memory>
#include <vector>

#include "map.h"
#include "player.h"

class PhysicsEngine {
public:
    static void movePlayer(Player& player, float dirX, float dirY, float deltaTime, Map& map);
    static bool shotHitPlayer(float originX, float originY, float dirX, float dirY, Map& map,
                              const Player& target, float maxDistance, float& impactDistance);
    static std::vector<std::shared_ptr<Player>> playersInAreaOfDamage(
            float originX, float originY, float dirX, float dirY, float areaAngle, float range,
            const std::vector<std::shared_ptr<Player>>& players);
    static float calculatePrecisionByDistance(float distance, float maxRange, float basePrecision);
};

#endif
