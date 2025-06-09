#include "../../../client/include/app-state/AppStateController.h"

#include <iostream>  //ELIMINAR
#include <optional>
#include <stdexcept>

#include "../../../client/include/app-state/GameMatchAppState.h"
#include "client/include/app-state/LobbyAppState.h"
#include "../../../client/include/app-state/LoginAppState.h"
#include "../../../client/include/app-state/MainMenuAppState.h"

AppStateController::AppStateController() { current_state = new LoginAppState(this); }

void AppStateController::update() {
    std::cout << "[AppStateController] Calling update on current state..." << std::endl;
    auto maybe_new_state = current_state->update();
    std::cout << "[AppStateController] Current state update completed" << std::endl;

    if (maybe_new_state.has_value()) {
        std::cout << "[AppStateController] New state requested: "
                  << static_cast<int>(maybe_new_state.value()) << std::endl;
        transition_to(maybe_new_state.value());
    } else {
        std::cout << "[AppStateController] No state transition requested" << std::endl;
    }
}

void AppStateController::transition_to(const AppStateCode& new_state) {
    std::cout << "[Controller] Transitioning from " << (current_state ? "current state" : "nullptr")
              << " to " << static_cast<int>(new_state) << std::endl;

    delete current_state;
    current_state = nullptr;

    switch (new_state) {
        case AppStateCode::MAIN_MENU:
            std::cout << "[Controller] Creating MainMenuAppState" << std::endl;
            current_state = new MainMenuAppState(this);
            break;
        }
        case AppStateCode::GAME_MATCH: {
            current_state = new GameMatchAppState(getClient());
            update();
        }
        case AppStateCode::LOBBY:
            std::cout << "[Controller] Creating LobbyAppState" << std::endl;
            current_state = new LobbyAppState(this);
            break;
        case AppStateCode::GAME_MATCH:
            std::cout << "[Controller] Creating GameMatchAppState" << std::endl;
            current_state = new GameMatchAppState(this);
            break;
        case AppStateCode::QUIT:
            std::cout << "[Controller] Quiting client program\n";
            return;
        default:
            throw std::runtime_error("Unknown app state.");
    }

    // std::cout << "[Controller] Setting controller in new state" << std::endl;
    // current_state->setController(this);

    std::cout << "[Controller] Calling update() on new state" << std::endl;
    update();
}

void AppStateController::setClient(std::unique_ptr<Client> c) {
    std::cout << "[AppStateController] setClient called" << std::endl;
    if (c == nullptr) {
        std::cerr << "[AppStateController] ERROR: Received nullptr client" << std::endl;
        return;
    }

    try {
        std::cout << "[AppStateController] Moving client pointer" << std::endl;
        client = std::move(c);
        std::cout << "[AppStateController] Client pointer moved successfully" << std::endl;
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
