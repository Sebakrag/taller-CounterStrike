#include "server.h"

#include <iostream>

#include "acceptor.h"
#include "game_manager.h"

Server::Server(const std::string& port): port(port), gameManager() {}


void Server::run() {
    Acceptor acceptor(port, gameManager);
    acceptor.start();

    std::string input;
    while (true) {
        std::getline(std::cin, input);
        if (input == "q" || input == "Q") {
            acceptor.stop();
            // acceptor.kill();
            acceptor.join();
            break;
        }
    }
}
