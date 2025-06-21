#include "../../include/weapon/fire_weapon.h"

#include <algorithm>
#include <iostream>
#include <random>

FireWeapon::FireWeapon(int damage, float price, int bullets, int rateOfFire, float basePrecision, float maxRange, float maxDamage):
        Weapon_(damage, maxDamage), price(price), bullets(bullets), rateOfFire(rateOfFire),
        lastShotTimeS(0), basePrecision(basePrecision), maxRange(maxRange), maxDamage(maxDamage) {}


bool FireWeapon::canShoot(double currentTime) const {
    if (bullets > 0)
        std::cout << "balas ok" << std::endl;
    std::cout << "currentTime:" << currentTime << std::endl;
    std::cout << "lastShotTimeS:" << lastShotTimeS << std::endl;
    std::cout << "getCooldownS:" << getCooldownS() << std::endl;
    return (bullets > 0) && ((currentTime - lastShotTimeS) >= getCooldownS());
}

double FireWeapon::getCooldownS() const {
    return 60.0f / rateOfFire;  // Balas por minuto a delay entre balas
}

void FireWeapon::addBullets(int amount) { bullets += amount; }

int FireWeapon::getBullets() const { return bullets; }

float FireWeapon::getBasePrecision() const { return basePrecision; }

float FireWeapon::getMaxRange() const { return maxRange; }

float FireWeapon::getMaxDamage() const {
    return maxDamage;
}

int FireWeapon::calculateDamage(float distance) const {
    float factor = 1.0f - (distance / maxRange);
    if (factor < 0.0f)
        factor = 0.0f;

    float scaledDamage = damage + (maxDamage - damage) * factor;

    // Variación aleatoria +- 10%
    float variation = scaledDamage * 0.1f;
    float lower = scaledDamage - variation;
    float upper = scaledDamage + variation;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(lower, upper);

    float finalDamage = dist(gen);
    finalDamage = std::clamp(finalDamage, static_cast<float>(damage), maxDamage);

    // std::cout << "Distancia: " << distance
    //       << ", Factor: " << factor
    //       << ", scaledDamage: " << scaledDamage
    //       << ", roll final: " << finalDamage
    //       << std::endl;
    return static_cast<int>(finalDamage);
}

