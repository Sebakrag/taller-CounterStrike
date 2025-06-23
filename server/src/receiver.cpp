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
            if (gameAction.type == GameActionType::ExitMatch) {
                std::cout << "jugador " << username << " se fue de la partida." << std::endl;
                stop();
            }
            //            std::cout << "Recibí una acción de " << username << std::endl;
            PlayerAction action(username, gameAction);
            queueActionsPlayers->push(action);
        }
    } catch (const ClosedQueue& e) {  // si la queue fue cerrada
        std::cerr << "Receiver: " << e.what() << std::endl;
        stop();
    } catch (const std::exception& e) {
        std::cerr << "Falla en el Receiver: " << e.what()
                  << std::endl;  // probablemente se murio el cliente
        PlayerAction action(username, GameAction(GameActionType::ExitMatch));
        queueActionsPlayers->push(action);
        kill();
        // protocol.shutDown(2);
        // stop();
    }
    std::cout << "Receiver out." << std::endl;
}

void Receiver::kill() {
    if (should_keep_running()) {
        stop();
        protocol.shutDown(2);
    }
    // queueActionsPlayers->close();
}
