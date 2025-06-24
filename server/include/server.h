#ifndef SERVER
#define SERVER

#include <string>
#include <vector>

#include "../../common/socket.h"

#include "acceptor.h"
#include "client_handler.h"
#include "yaml_config.h"

class Server {
private:
    std::string port;
    GameManager gameManager;
    // Acceptor acceptor;
    // Socket srv;
    // std::vector<ClientHandler*> handlers;

    void initConstants(const YamlConfig& yamlConfig) const;

public:
    explicit Server(const std::string& port, const std::string& pathConfigYaml);

    void run();

    ~Server() = default;
};

#endif
