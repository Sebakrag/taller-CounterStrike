#ifndef ACCEPTOR_H_
#define ACCEPTOR_H_

#include <list>
#include <string>

#include "../common/socket.h"
#include "../common/thread.h"

#include "client_handler.h"
#include "game_manager.h"

class Acceptor: public Thread {
private:
    Socket skt_server;
    std::list<ClientHandler> clients;

    GameManager& gameManager;

public:
    Acceptor(const std::string& port, GameManager& gameManager);

    void run() override;

    void stop() override;

private:
    /*
     * Revisa todos los clientHandlers de la lista de clients
     * y elimina a aquellos que hayan finalizado.
     */
    void reapDeadClients();

    /*
     * Interrumpe todos los hilos clientHandlers de la lista de clients
     * y los elimina.
     */
    void killAll();
};


#endif  // ACCEPTOR_H_
