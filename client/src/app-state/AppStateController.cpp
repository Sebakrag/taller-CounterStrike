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
        case AppStateCode::MAIN_MENU: {
            current_state = new MainMenuAppState();
            update();
            break;
        }
        case AppStateCode::GAME_MATCH: {
            current_state = new GameMatchAppState();
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

AppStateController::~AppStateController() {
    std::cout << "executing AppStateController destructor..." << std::endl;
    if (current_state != nullptr) {
        std::cout << "Deleting the current_state" << std::endl;
        delete current_state;
        std::cout << "current_state deleted" << std::endl;
    }
    std::cout << "AppStateController destructor finish" << std::endl;
}
