#ifndef RECEIVER_H_
#define RECEIVER_H_

#include <memory>
#include <string>

#include "../../common/queue.h"
#include "../../common/thread.h"
#include "../../common/types.h"

#include "server_protocol.h"

// idea.
// recibe por el protocolo la GameAction.
// crea la PlayerAction y la carga en la queue de PlayerActions
// Importante. No cerrar la queue aquí nunca. De eso se encarga el gameloop
class Receiver: public Thread {
private:
    std::string username;
    ServerProtocol& protocol;
    std::shared_ptr<Queue<PlayerAction>> queueActionsPlayers;  // recurso compartido
    bool running;

public:
    Receiver(const std::string& username, ServerProtocol& protocol,
             std::shared_ptr<Queue<PlayerAction>>);

    void run() override;

    void kill();
};

#endif  // RECEIVER_H_
