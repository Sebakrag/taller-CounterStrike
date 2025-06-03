#include "client/include/app-state/AppStateController.h"

#include <iostream>  //ELIMINAR
#include <optional>
#include <stdexcept>

#include "client/include/app-state/GameMatchAppState.h"
#include "client/include/app-state/LoginAppState.h"
#include "client/include/app-state/MainMenuAppState.h"

AppStateController::AppStateController() { current_state = new LoginAppState(); }

void AppStateController::update() {
    auto maybe_new_state = current_state->update();
    if (maybe_new_state.has_value()) {
        transition_to(maybe_new_state.value());
    }
}

void AppStateController::transition_to(const AppStateCode& new_state) {
    delete current_state;
    current_state = nullptr;

    switch (new_state) {
        case AppStateCode::MAIN_MENU:
            current_state = new MainMenuAppState();
            break;
        case AppStateCode::LOBBY:
            current_state = new LobbyAppState();
            break;
        case AppStateCode::GAME_MATCH:
            current_state = new GameMatchAppState();
            break;
        case AppStateCode::QUIT:
            std::cout << "[Controller] Quiting client program\n";
            return;
        default:
            throw std::runtime_error("Unknown app state.");
    }

    current_state->setController(this);
    update();
}

void AppStateController::setClient(std::shared_ptr<Client> c) {
    client = std::move(c);
}

Client* AppStateController::getClient() const {
    return client.get();
}

AppStateController::~AppStateController() {
    std::cout << "executing AppStateController destructor..." << std::endl;
    if (current_state != nullptr) {
        std::cout << "Deleting the current_state" << std::endl;
        delete current_state;
        std::cout << "current_state deleted" << std::endl;
    }
    std::cout << "AppStateController destructor finish" << std::endl;
}
