#include "../include/receiver.h"

#include <iostream>

Receiver::Receiver(const std::string& username, ServerProtocol& protocol,
                   Queue<PlayerAction>& queueActionsPlayers):
        username(username), protocol(protocol), queueActionsPlayers(queueActionsPlayers) {}

void Receiver::run() {
    std::cout << "Receiver run..." << std::endl;
    try {
        while (should_keep_running()) {
            GameAction gameAction = protocol.recvGameAction();  // bloqueante
            std::cout << "Recibí una acción de " << username << std::endl;
            PlayerAction action(username, gameAction);
            queueActionsPlayers.push(action);  // capaz que try_push es mejor
        }
    } catch (...) {
        stop();
    }
}

void Receiver::kill() {}
