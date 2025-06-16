#include "../include/physics_engine.h"

void PhysicsEngine::movePlayer(Player& player, float dirX, float dirY, float deltaTime, Map& map) {
    if (dirX == 0 && dirY == 0)
        return;

    float radius = 15.0f; //Radio del hitbox

    // Normalizamos la direccion
    const float length = std::sqrt((dirX * dirX) + (dirY * dirY));
    const float normDirX = dirX / length;
    const float normDirY = dirY / length;

    // Calculamos el desplazamiento
    const float dx = normDirX * player.getSpeed() * deltaTime;
    const float dy = normDirY * player.getSpeed() * deltaTime;

    // Nueva posición objetivo
    const float newX = player.getX() + dx;
    const float newY = player.getY() + dy;

    // Validamos X e Y por separado para permitir movimiento en las esquinas
    if (map.isWalkable(static_cast<int>(newX + radius), static_cast<int>(player.getY())) &&
        map.isWalkable(static_cast<int>(newX - radius), static_cast<int>(player.getY()))) {
        player.setX(newX);
    }

    if (map.isWalkable(static_cast<int>(player.getX()), static_cast<int>(newY + radius)) &&
        map.isWalkable(static_cast<int>(player.getX()), static_cast<int>(newY - radius))) {
        player.setY(newY);
    }
}

bool PhysicsEngine::shotHitPlayer(float originX, float originY, float dirX, float dirY, Map& map,
                                  const Player& target, float maxDistance, float& impactDistance) {
    // Normalizar la dirección
    const float len = std::sqrt(dirX * dirX + dirY * dirY);
    if (len == 0.0f)
        return false;
    dirX /= len;
    dirY /= len;

    float stepSize = 0.1f;
    float x = originX;
    float y = originY;
    float distance = 0.0f;

    float targetRadius = 15.0f;

    while (distance < maxDistance) {
        // verificamos colision con el mapa
        if (!map.isWalkable(static_cast<int>(x), static_cast<int>(y))) {
            return false;
        }

        const float dx = target.getX() - x;
        const float dy = target.getY() - y;
        float distToTarget = std::sqrt(dx * dx + dy * dy);

        if (distToTarget < targetRadius) {  // radio del hitbox
            impactDistance = distance;
            return true;
        }

        x += dirX * stepSize;
        y += dirY * stepSize;
        distance += stepSize;
    }

    return false;
}


float PhysicsEngine::calculatePrecisionByDistance(float distance, float maxRange,
                                                  float basePrecision) {
    if (maxRange <= 0.0f)
        return 0.0f;

    float factor = 1.0f - (distance / maxRange);
    if (factor < 0.0f)
        factor = 0.0f;

    return basePrecision * factor;
}

bool PhysicsEngine::knifeHit(float originX, float originY, float dirX, float dirY, const Player& target, float& outDist) {
    float dx = target.getX() - originX;
    float dy = target.getY() - originY;

    float distance = std::sqrt(dx * dx + dy * dy);
    if (distance > 50.0f)
        return false;

    float dot = dx * dirX + dy * dirY;
    if (dot < 0)
        return false;

    outDist = distance;
    return true;
}

bool PhysicsEngine::playerTouchingItem(float playerPosX, float playerPosY, float itemPosX, float itemPosY) {
    const float radius = 15.0f;
    const float dx = playerPosX - itemPosX;
    const float dy = playerPosY - itemPosY;

    return (dx * dx + dy * dy) <= (radius * radius);
}

