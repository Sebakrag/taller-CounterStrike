#include "../include/server.h"

#include <iostream>
#include <map>

#include "../include/acceptor.h"
#include "../include/game_manager.h"
#include "../include/scenario_registry.h"

Server::Server(const std::string& port, const std::string& pathConfigYaml):
        port(port), gameManager() {
    // hardcodeo. En realidad tengo leer del config.YAML
    YamlConfig yamlConfig(pathConfigYaml);

    initConstants(yamlConfig);

    // window config.
    WindowConfig window_config(yamlConfig.getWindowWidth(), yamlConfig.getWindowHeight(),
                               yamlConfig.isFullscreen() ? 1 : 0);

    FOVConfig fov_config(yamlConfig.getFovConfig());
    // maps
    std::map<std::string, TileMap> scenarios;
    scenarios.emplace("demo", TileMap::getLevelDemo());
    ScenarioRegistry::init(window_config, fov_config, scenarios);
}

void Server::run() {
    Acceptor acceptor(port, gameManager);
    acceptor.start();

    std::string input;
    while (true) {
        std::getline(std::cin, input);
        if (input == "q" || input == "Q") {
            gameManager.killAllMatchs();
            acceptor.stop();
            // acceptor.kill();
            acceptor.join();
            break;
        }
    }
}
void Server::initConstants(const YamlConfig& yamlConfig) const {
    MatchRoom::init(yamlConfig.getMaxPlayers());
    Match::init(yamlConfig.getRoundDuration(), yamlConfig.getPreparationTime(),
                yamlConfig.getMaxRounds());

    Player::init(yamlConfig.getPlayerSpeed(), yamlConfig.getPlayerMoney(),
                 yamlConfig.getPlayerHealth());

    Bomb::init(yamlConfig.getBombTimeToExplode());

    // weapons.
    auto ak47 = yamlConfig.getWeaponData("ak47");
    WeaponAk47::init(ak47.damage, ak47.price, ak47.bullets, ak47.rateOfFire, ak47.precision, ak47.maxRange);

    auto awp = yamlConfig.getWeaponData("awp");
    WeaponAwp::init(awp.damage, awp.price, awp.bullets, awp.rateOfFire, awp.precision, awp.maxRange);

    auto m3 = yamlConfig.getWeaponData("m3");
    WeaponM3::init(m3.damage, m3.price, m3.bullets, m3.rateOfFire, m3.precision, m3.maxRange);

    auto glock = yamlConfig.getWeaponData("glock");
    WeaponGlock::init(glock.damage, glock.bullets, glock.rateOfFire, glock.precision, glock.maxRange);

    auto knife = yamlConfig.getWeaponData("knife");
    WeaponKnife::init(knife.damage);
    // WeaponAk47::init(35, 2700, 30, 24);
    // WeaponAwp::init(100, 3500, 10, 20);
    // WeaponGlock::init(45, 40, 700);
    // WeaponKnife::init(20);
    // WeaponM3::init(70, 3000, 20, 60);
}
