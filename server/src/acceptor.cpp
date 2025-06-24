#include "../include/acceptor.h"

#include <utility>

#include "../include/client_handler.h"
#include "../include/game_manager.h"
#include "../include/server_protocol.h"

Acceptor::Acceptor(const std::string& port, GameManager& gameManager):
        skt_server(port.c_str()), gameManager(gameManager) {}

void Acceptor::run() {

    try {
        while (should_keep_running()) {
            Socket peer = skt_server.accept();
            reapDeadClients();

            ServerProtocol protocol(std::move(peer));
            std::string username = protocol.recvUsername();

            // chequeo si el nombre de usuario está disponible
            bool user_avaiable = true;
            for (auto& c: clients) {
                if (username == c.getUsername()) {
                    user_avaiable = false;
                    break;
                }
            }
            protocol.sendConfirmation(user_avaiable);

            if (user_avaiable) {
                clients.emplace_back(std::move(protocol), username, gameManager);
            }
            reapDeadClients();
        }

    } catch (const std::exception& e) {
        reapDeadClients();
        std::cerr << "Acceptor: " << e.what() << std::endl;
    }
    killAll();
}

void Acceptor::reapDeadClients() {
    clients.remove_if([](ClientHandler& c) {
        if (!c.is_alive()) {
            c.join();
            return true;
        }
        return false;
    });
}
void Acceptor::killAll() {
    for (auto& c: clients) {
        c.kill();
        c.join();
    }
    clients.clear();
}

void Acceptor::stop() { skt_server.shutdown(2); }
