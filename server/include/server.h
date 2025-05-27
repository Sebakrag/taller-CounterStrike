#ifndef SERVER
#define SERVER

#include <string>
#include <vector>

#include "../../common/socket.h"

#include "acceptor.h"
#include "client_handler.h"

class Server {
private:
    std::string port;
    GameManager gameManager;
    // Acceptor acceptor;
    // Socket srv;
    // std::vector<ClientHandler*> handlers;

public:
    explicit Server(const std::string& port);

    void run();

    ~Server() = default;
};

#endif
