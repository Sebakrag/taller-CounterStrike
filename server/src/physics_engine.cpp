#include "../include/physics_engine.h"

void PhysicsEngine::movePlayer(Player& player, float dirX, float dirY, float deltaTime,
                                Map& map) {
    if (dirX == 0 && dirY == 0)
        return;

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
    if (map.isWalkable(static_cast<int>(newX), static_cast<int>(player.getY()))) {
        player.setX(newX);
    }

    if (map.isWalkable(static_cast<int>(player.getX()), static_cast<int>(newY))) {
        player.setY(newY);
    }
}

bool PhysicsEngine::shotHitPlayer(float originX, float originY, float dirX, float dirY,
                                    Map& map, const Player& target, float maxDistance,
                                  float& impactDistance) {
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

    while (distance < maxDistance) {
        // verificamos colision con el mapa
        if (!map.isWalkable(static_cast<int>(x), static_cast<int>(y))) {
            return false;
        }

        const float dx = target.getX() - x;
        const float dy = target.getY() - y;
        float distToTarget = std::sqrt(dx * dx + dy * dy);

        if (distToTarget < 0.3f) {  // radio del hitbox
            impactDistance = distance;
            return true;
        }

        x += dirX * stepSize;
        y += dirY * stepSize;
        distance += stepSize;
    }

    return false;
}

std::vector<std::shared_ptr<Player>> PhysicsEngine::playersInAreaOfDamage(
        float originX, float originY, float dirX, float dirY, float areaAngle, float range,
        const std::vector<std::shared_ptr<Player>>& players) {
    std::vector<std::shared_ptr<Player>> damagedPlayers;

    const float magDir = std::sqrt(dirX * dirX + dirY * dirY);
    if (magDir == 0.0f)
        return damagedPlayers;
    dirX /= magDir;
    dirY /= magDir;

    for (const auto& target: players) {
        if (!target->isAlive())
            continue;

        float dx = target->getX() - originX;
        float dy = target->getY() - originY;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance > range)
            continue;

        const float magVec = std::sqrt(dx * dx + dy * dy);
        if (magVec == 0.0f) {
            damagedPlayers.push_back(target);
            continue;
        }

        dx /= magVec;
        dy /= magVec;

        const float cosTheta = dirX * dx + dirY * dy;
        float angle = std::acos(cosTheta);

        if (angle <= areaAngle / 2.0f)
            damagedPlayers.push_back(target);
    }

    return damagedPlayers;
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
