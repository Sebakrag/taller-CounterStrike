#include "client/include/app-state/LoginAppState.h"

#include <iostream>  // Eliminar una vez tengamos la parte grafica.

LoginAppState::LoginAppState() {
    // Here we can initialize some music. This applies only if we
    // are using the heap to create the app states.
    // Otherwise, we should override the enter() method of AppState, and
    // there we should initialize the music.
}

// std::optional<AppStateCode> LoginAppState::update() {
// struct state_result_t {
//     std::optional<AppStateCode> app_state_code;
//     Client client;
// };

// state_result_t LoginAppState::update() {
std::optional<AppStateCode> LoginAppState::update() {
    LoginWindow dlg;
    if (dlg.exec() == QDialog::Accepted) {
        const std::string usr = dlg.userName().toStdString();
        const std::string ip  = dlg.serverIp() .toStdString();
        const std::string port = dlg.serverPort().toStdString();

        std::cout << "[LoginAppState] Usuario=" << usr
                  << " Servidor=" << ip << std::endl;

        try {
            auto c = std::make_shared<Client>(ip, port, usr);
            std::cout << "[LoginAppState] c" << c
                             << std::endl;

            controller->setClient(c);
            return AppStateCode::MAIN_MENU;
        }
        catch(const std::exception &e) {
            std::cerr << "Login: " << e.what() << std::endl;
            return AppStateCode::LOGIN;
        }
    }
    return AppStateCode::LOGIN;
}

LoginAppState::~LoginAppState() {
}
