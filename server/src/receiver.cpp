#include "../include/receiver.h"

#include <iostream>

Receiver::Receiver(const std::string& username, ServerProtocol& protocol,
                   std::shared_ptr<Queue<PlayerAction>> queueActionsPlayers):
        username(username), protocol(protocol), queueActionsPlayers(queueActionsPlayers) {}

void Receiver::run() {
    std::cout << "Receiver run..." << std::endl;
    try {
        while (should_keep_running()) {
            GameAction gameAction = protocol.recvGameAction();  // bloqueante
            std::cout << "Recibí una acción de " << username << std::endl;
            PlayerAction action(username, gameAction);
            queueActionsPlayers->push(action);  // capaz que try_push es mejor
        }
    } catch (const ClosedQueue& e) {  // si la queue fue cerrada
        std::cerr << "Receiver: " << e.what() << std::endl;
        stop();
    } catch (const std::exception& e) {
        std::cerr << "Error en el Receiver: " << e.what() << std::endl;
        stop();
    }
    std::cout << "Receiver out." << std::endl;
}

void Receiver::kill() {
    // queueActionsPlayers->close();
    protocol.shutDown(2);
}
