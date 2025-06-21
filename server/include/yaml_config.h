#pragma once
#include <string>

#include <yaml-cpp/yaml.h>

#include "../../common/dtos/MatchInfo.h"

class YamlConfig {
public:
    explicit YamlConfig(const std::string& filepath);

    // MatchRoom
    int getMaxPlayers() const;

    // Match
    float getRoundDuration() const;
    float getPreparationTime() const;
    int getMaxRounds() const;

    // Player
    float getPlayerSpeed() const;
    int getPlayerMoney() const;
    int getPlayerHealth() const;

    // Weapons
    struct WeaponData {
        int damage;
        int price = 0;
        int bullets;
        int rateOfFire;
        float precision;
        float maxRange;
    };

    WeaponData getWeaponData(const std::string& weaponName) const;

    // Bomb
    int getBombTimeToExplode() const;

    // Window
    int getWindowWidth() const;
    int getWindowHeight() const;
    bool isFullscreen() const;

    // fov
    FOVConfig getFovConfig();

private:
    YAML::Node config;
};
