#ifndef SENDER_H_
#define SENDER_H_

#include <memory>
#include <string>

#include "../../common/queue.h"
#include "../../common/thread.h"
#include "../../common/types.h"

#include "server_protocol.h"

class Sender: public Thread {
private:
    ServerProtocol& protocol;
    std::shared_ptr<Queue<GameInfo>> senderQueue;
    bool running;

public:
    Sender(ServerProtocol& protocol, std::shared_ptr<Queue<GameInfo>> senderQueue);

    void run() override;

    void kill();
};

#endif  // SENDER_H_
