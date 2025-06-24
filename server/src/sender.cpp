#include "../include/sender.h"

#include <iostream>

Sender::Sender(ServerProtocol& protocol, std::shared_ptr<Queue<GameInfo>> senderQueue):
        protocol(protocol), senderQueue(senderQueue) {}

void Sender::run() {
    try {
        while (should_keep_running()) {
            GameInfo gameInfo = senderQueue->pop();

            protocol.sendGameInfo(gameInfo);
        }
    } catch (const ClosedQueue& e) {  // si la queue fue cerrada
        std::cerr << "Sender: " << e.what() << std::endl;
        protocol.shutDown(2);
        stop();
    } catch (const std::exception& e) {
        std::cerr << "Error en el Sender: " << e.what() << std::endl;
        protocol.shutDown(2);
        stop();
    }
}

void Sender::kill() {
    senderQueue->close();
    stop();
    // protocol.shutDown(2);
}
