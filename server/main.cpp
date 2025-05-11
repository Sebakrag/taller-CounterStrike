
#include <iostream>
#include "server.h"

int main(const int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    const std::string port = argv[1];

    try {
        Server server(port);
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Error starting the server: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
