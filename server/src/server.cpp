#include "../include/server.h"

#include <iostream>

#include "../include/acceptor.h"
#include "../include/game_manager.h"

Server::Server(const std::string& port): port(port), gameManager("maps") {  // Directorio por defecto para mapas
    MatchRoom::init(10);  // hardcodeo. Enrealidad tengo leer del config.YAML
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
