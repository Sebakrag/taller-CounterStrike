#include "../include/client_sender.h"

#include <iostream>

ClientSender::ClientSender(ClientProtocol& protocol, Queue<GameAction>& send_queue):
        protocol(protocol), send_queue(send_queue) {}
void ClientSender::run() {
    std::cout << "sender start" << std::endl;
    try {
        while (should_keep_running()) {
            GameAction gameAction = send_queue.pop();
            protocol.sendGameAction(gameAction);
        }
    } catch (const ClosedQueue& e) {  // si la queue fue cerrada
        stop();
    } catch (const std::exception& e) {
        std::cerr << "Error en el Sender: " << e.what() << std::endl;
    }
}

void ClientSender::kill() {
    send_queue.close();
    // protocol.shutDown(2);
}
