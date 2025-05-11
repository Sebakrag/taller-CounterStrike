#include "client_handler.h"
#include <iostream>

ClientHandler::ClientHandler(Socket peer)
    : peer(std::move(peer)){}

void ClientHandler::run() {
    try {
        std::cout << "Handler started. Waiting clients message...\n";

    } catch (const std::exception& e) {
        std::cerr << "ClientHandler error: " << e.what() << std::endl;
    }
}
