#include "../include/client_receiver.h"

#include <iostream>
#include <ostream>
#include <string>

ClientReceiver::ClientReceiver(ClientProtocol& protocol, Queue<GameInfo>& recv_queue):
        protocol(protocol), recv_queue(recv_queue) {}

void ClientReceiver::run() {
    std::cout << "Receiver start" << std::endl;
    double seconds = 0;
    try {
        while (should_keep_running()) {
            GameInfo gameInfo = protocol.recvGameInfo();
            if (gameInfo.gamePhase == GamePhase::Combat) {
                std::cout << "empieza el combate." << std::endl;
            } else if (gameInfo.gamePhase == GamePhase::Preparation &&
                       seconds != gameInfo.timeLeft) {
                std::cout << "empieza en: " << std::to_string(seconds) << std::endl;
                seconds = gameInfo.timeLeft;
            }
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
