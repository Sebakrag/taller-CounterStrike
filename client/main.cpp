#include <exception>
#include <iostream>

#include "common/constants.h"
#include "include/app-state/AppStateController.h"
#include <QApplication>
#include "common/constants.h"

int main() {
    try {
        AppStateController app_sc;
        app_sc.update();

        std::cout << "AppStateController finish his job" << std::endl;
        return SUCCESS;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return ERROR;
    }
}
/*
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
    //auto& queueActions = client.getSendeQueue();
    //queueActions.push(GameAction(GameActionType::BuyWeapon, Weapon::Ak47));
    client.mainLoop();
}
 **/
