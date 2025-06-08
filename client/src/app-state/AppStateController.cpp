#include "../../../client/include/app-state/AppStateController.h"

#include <iostream>  //ELIMINAR
#include <optional>
#include <stdexcept>

#include "../../../client/include/app-state/GameMatchAppState.h"
#include "../../../client/include/app-state/LoginAppState.h"
#include "../../../client/include/app-state/MainMenuAppState.h"

AppStateController::AppStateController() { current_state = new LoginAppState(this); }

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
        case AppStateCode::MAIN_MENU: {
            current_state = new MainMenuAppState();
            update();
            break;
        }
        case AppStateCode::GAME_MATCH: {

            current_state = new GameMatchAppState(getClient());
            update();
            break;
        }
        case AppStateCode::QUIT: {
            std::cout << "Quiting client program" << std::endl;
            break;
        }
        default:
            throw std::runtime_error("Unknown app state.");
    }
}

void AppStateController::setClient(std::unique_ptr<Client> c) {
    if (c == nullptr) {
        std::cerr << "[AppStateController] ERROR: Received nullptr client" << std::endl;
        return;
    }

    try {
        client = std::move(c);
    } catch (const std::exception& e) {
        std::cerr << "[AppStateController] Exception in setClient: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "[AppStateController] Unknown exception in setClient" << std::endl;
    }
}

Client* AppStateController::getClient() const {
    std::cout << "[AppStateController] getClient called" << std::endl;
    if (!client) {
        std::cerr << "[AppStateController] WARNING: Returning nullptr from getClient" << std::endl;
        return nullptr;
    }
    std::cout << "[AppStateController] Returning valid client pointer" << std::endl;
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
