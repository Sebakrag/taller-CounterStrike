#include "../include/server.h"

#include <iostream>
#include <map>

#include "../include/acceptor.h"
#include "../include/game_manager.h"
#include "../include/scenario_registry.h"

Server::Server(const std::string& port): port(port), gameManager() {
    // hardcodeo. En realidad tengo leer del config.YAML

    MatchRoom::init(10);
    Match::init(120.0, 5.0, 8);

    WindowConfig window_config(700, 400, 0);
    std::map<std::string, TileMap> scenarios;
    scenarios.emplace("demo", TileMap::getLevelDemo());
    ScenarioRegistry::init(window_config, scenarios);
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
