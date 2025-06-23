#include "../../../client/include/app-state/AppStateController.h"

#include <optional>
#include <stdexcept>

#include "../../../client/include/app-state/GameMatchAppState.h"
#include "../../../client/include/app-state/LobbyAppState.h"
#include "../../../client/include/app-state/LoginAppState.h"
#include "../../../client/include/app-state/MainMenuAppState.h"

AppStateController::AppStateController() { current_state = new LoginAppState(this); }

void AppStateController::update() {
    auto maybe_new_state = current_state->update();

    if (maybe_new_state.has_value()) {
        transition_to(maybe_new_state.value());
    } else {
        std::cout << "[AppStateController] No state transition requested" << std::endl;
    }
}

void AppStateController::transition_to(const AppStateCode& new_state) {
    delete current_state;
    current_state = nullptr;

    switch (new_state) {
        case AppStateCode::MAIN_MENU: {
            current_state = new MainMenuAppState(this);
            break;
        }
        case AppStateCode::LOBBY:
            current_state = new LobbyAppState(this);
            break;
        case AppStateCode::GAME_MATCH:
            current_state = new GameMatchAppState(this);
            break;
        case AppStateCode::QUIT:
            std::cout << "[Controller] Quiting client program\n";
            return;
        default:
            throw std::runtime_error("Unknown app state.");
    }

    update();
}

void AppStateController::setClient(std::unique_ptr<Client> c) {
    if (c == nullptr) {
        std::cerr << "[AppStateController] ERROR: Received nullptr client" << std::endl;
        return;
    }

    try {
        client = std::move(c);
    } catch (const std::exception& e) {
        std::cerr << "[AppStateController] ERROR: Exception in setClient: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "[AppStateController] ERROR: Unknown exception in setClient" << std::endl;
    }
}

Client* AppStateController::getClient() const {
    if (!client) {
        std::cerr << "[AppStateController] WARNING: Returning nullptr from getClient" << std::endl;
        return nullptr;
    }
    return client.get();
}

AppStateController::~AppStateController() {
    if (current_state != nullptr) {
        delete current_state;
    }
}
