#pragma once

#include "common/socket.h"
#include "common/threads.h"

class ClientHandler : public Thread {
private:
    Socket peer;

public:
    ClientHandler(Socket peer);

    void run() override;
};