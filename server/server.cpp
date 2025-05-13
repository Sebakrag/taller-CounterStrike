#include "server.h"

#include <iostream>
#include <utility>

Server::Server(const std::string& port): port(port), srv(port.c_str()) {}

void Server::run() {
    std::cout << "Server listening port " << port << std::endl;

    try {
        while (true) {
            Socket peer = srv.accept();

            std::cout << "New client connected. " << std::endl;

            auto* handler = new ClientHandler(std::move(peer));
            handler->start();
            handlers.push_back(handler);
        }

    } catch (const std::exception& e) {
        std::cerr << "Critical server error: " << e.what() << std::endl;
    }

    for (auto* handler: handlers) {
        handler->join();
        delete handler;
    }
}
