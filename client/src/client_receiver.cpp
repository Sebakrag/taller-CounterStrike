#include "../include/client_receiver.h"

#include <iostream>
#include <ostream>

ClientReceiver::ClientReceiver(ClientProtocol& protocol, Queue<GameInfo>& recv_queue):
        protocol(protocol), recv_queue(recv_queue) {}

void ClientReceiver::run() {
    std::cout << "Receiver start" << std::endl;
    try {
        while (should_keep_running()) {
            GameInfo gameInfo = protocol.recvGameInfo();
            std::cout << "GameInfo..." << std::endl;
            recv_queue.push(gameInfo);
        }
    } catch (const ClosedQueue& e) {  // si la queue fue cerrada
        stop();
    } catch (const std::exception& e) {
        std::cerr << "Error en el Receiver: " << e.what() << std::endl;
        stop();
    }
}

void ClientReceiver::kill() {
    recv_queue.close();
    // protocol.shutDown(2);
}
