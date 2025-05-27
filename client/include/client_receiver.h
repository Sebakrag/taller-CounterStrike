#ifndef CLIENT_RECEIVER_H_
#define CLIENT_RECEIVER_H_

#include <atomic>

#include "../../common/queue.h"
#include "../../common/socket.h"
#include "../../common/thread.h"
#include "../../common/types.h"

#include "client_protocol.h"


class ClientReceiver: public Thread {
private:
    ClientProtocol& protocol;
    Queue<GameInfo>& recv_queue;

public:
    ClientReceiver(ClientProtocol& protocol, Queue<GameInfo>& recv_queue);

    void run() override;

    void kill();
};

#endif  // CLIENT_RECEIVER_H_
