#include "../include/physics_engine.h"

void PhysicsEngine::movePlayer(Player& player, float dirX, float dirY, float deltaTime, Map& map) {
    if (dirX == 0 && dirY == 0)
        return;

    float radius = 15.0f;  // Radio del hitbox

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


bool PhysicsEngine::shotHitPlayer(float projX, float projY,
                                  const Player& target, const FireWeapon& weapon, float& impactDistance) {
    static std::default_random_engine rng(std::random_device{}());
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    float dx = target.getX() - projX;
    float dy = target.getY() - projY;

    float distance = std::sqrt(dx * dx + dy * dy);
    float targetRadius = 15.0f;

    if (distance < targetRadius) {
        impactDistance = distance;

        float precision = calculatePrecisionByDistance(distance, weapon.getMaxRange(), weapon.getBasePrecision());
        float roll = dist(rng);

        if (roll <= precision) {
            std::cout << "Disparo ACERTADO - precision: " << precision << ", roll: " << roll << std::endl;
            return true;
        } else {
            std::cout << "Disparo FALLADO - precision: " << precision << ", roll: " << roll << std::endl;
            return false;
        }
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

bool PhysicsEngine::knifeHit(float originX, float originY, float dirX, float dirY,
                             const Player& target, float& outDist) {
    float dx = target.getX() - originX;
    float dy = target.getY() - originY;

    float distance = std::sqrt(dx * dx + dy * dy);
    if (distance > 15.0f)
        return false;

    float dot = dx * dirX + dy * dirY;
    if (dot < 0)
        return false;

    outDist = distance;
    return true;
}

bool PhysicsEngine::playerTouchingItem(float playerPosX, float playerPosY, float itemPosX,
                                       float itemPosY) {
    const float radius = RADIUS_PLAYER_TOUCH;
    const float dx = playerPosX - itemPosX;
    const float dy = playerPosY - itemPosY;

    return (dx * dx + dy * dy) <= (radius * radius);
}
