#include "../include/client_receiver.h"

ClientReceiver::ClientReceiver(ClientProtocol& protocol, Queue<GameInfo>& recv_queue):
        protocol(protocol), recv_queue(recv_queue) {}
void ClientReceiver::run() {
    try {
        while (should_keep_running()) {
            GameInfo gameInfo = protocol.recvGameInfo();

            recv_queue.push(gameInfo);
        }
    } catch (const ClosedQueue& e) {  // si la queue fue cerrada
        stop();
    } catch (const std::exception& e) {
        std::cerr << "Error en el Receiver: " << e.what() << std::endl;
    }
}

void ClientReceiver::kill() {
    recv_queue.close();
    // protocol.shutDown(2);
}
