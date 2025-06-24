#include "../../server/include/yaml_config.h"

#include <stdexcept>

YamlConfig::YamlConfig(const std::string& filepath) { config = YAML::LoadFile(filepath); }

// MatchRoom
int YamlConfig::getMaxPlayers() const { return config["match_room"]["max_players"].as<int>(); }

// Match
float YamlConfig::getRoundDuration() const { return config["match"]["round_duration"].as<float>(); }

float YamlConfig::getPreparationTime() const {
    return config["match"]["preparation_time"].as<float>();
}

int YamlConfig::getMaxRounds() const { return config["match"]["max_rounds"].as<int>(); }

// Player
float YamlConfig::getPlayerSpeed() const { return config["player"]["speed"].as<float>(); }

int YamlConfig::getPlayerMoney() const { return config["player"]["money"].as<int>(); }

int YamlConfig::getPlayerHealth() const { return config["player"]["health"].as<int>(); }

// Weapon
YamlConfig::WeaponData YamlConfig::getWeaponData(const std::string& weaponName) const {
    auto node = config["weapons"][weaponName];
    if (!node)
        throw std::runtime_error("Weapon not found: " + weaponName);

    WeaponData data;
    data.damage = node["damage"].as<int>();
    data.bullets = node["bullets"] ? node["bullets"].as<int>() : 0;
    data.price = node["price"] ? node["price"].as<int>() : 0;
    data.rateOfFire = node["rate_of_fire"] ? node["rate_of_fire"].as<int>() : 0;
    data.precision = node["precision"] ? node["precision"].as<float>() : 0;
    data.maxRange = node["max_range"] ? node["max_range"].as<float>() : 0;
    data.maxDamage = node["max_damage"] ? node["max_damage"].as<int>() : 0;
    return data;
}

// Bomb
int YamlConfig::getBombTimeToExplode() const { return config["bomb"]["time_to_explode"].as<int>(); }

// Window
int YamlConfig::getWindowWidth() const { return config["window"]["width"].as<int>(); }

int YamlConfig::getWindowHeight() const { return config["window"]["height"].as<int>(); }

bool YamlConfig::isFullscreen() const { return config["window"]["fullscreen"].as<int>() != 0; }


FOVConfig YamlConfig::getFovConfig() {
    bool active = config["fov"]["active"].as<int>() != 0;
    auto w = getWindowWidth();
    auto h = getWindowHeight();
    int radius = config["fov"]["circle_radius"].as<int>();
    float angle = config["fov"]["angle"].as<float>();
    int visibility_distance = config["fov"]["visibility_distance"].as<int>();
    float transparency = config["fov"]["transparency"].as<float>();

    return FOVConfig(active, w, h, radius, angle, visibility_distance, transparency);
}
