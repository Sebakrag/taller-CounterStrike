#ifndef CLIENT_SENDER_H_
#define CLIENT_SENDER_H_

#include <atomic>

#include "../../common/queue.h"
#include "../../common/socket.h"
#include "../../common/thread.h"
#include "../../common/types.h"

#include "client_protocol.h"


class ClientSender: public Thread {
private:
    ClientProtocol& protocol;
    Queue<GameAction>& send_queue;

public:
    ClientSender(ClientProtocol& protocol, Queue<GameAction>& send_queue);

    void run() override;

    void kill();
};

#endif
