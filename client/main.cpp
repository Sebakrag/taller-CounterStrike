#include <exception>
#include <iostream>
#include <string>

#include "client.h"

//#include <SDL2/SDL.h>
//#include <SDL2pp/SDL2pp.hh>

// using namespace SDL2pp;

// ejemplo para correr: ./client localhost 8080 usuario1
int main(int argc, const char* argv[]) try {
    if (argc != 4) {
        std::cerr << "cantidad de argumentos incorrecta" << std::endl;
        return 1;
    }
    std::string hostname = argv[1];
    std::string servname = argv[2];
    std::string username = argv[3];

    Client client(hostname, servname, username);
    client.mainLoop();

} catch (std::exception& e) {
    // If case of error, print it and exit with error
    std::cerr << e.what() << std::endl;
    return 1;
}
