#include "app_state/LobbyAppState.h"

#include <iostream>
#include <optional>

#include "app_state/AppStateCode.h"
#include "app_state/AppStateController.h"
#include "client.h"
#include "ui/LobbyWindow.h"

extern Client* g_client;

LobbyAppState::LobbyAppState() {
}

std::optional<AppStateCode> LobbyAppState::update() {
    Client* client = controller->getClient();
    if (!client) {
        std::cerr << "[LobbyAppState] Error: No client available" << std::endl;
        return AppStateCode::MAIN_MENU;
    }

    g_client = client;

    if (client->getStatus() != InLobby && client->getStatus() != InGame) {
        std::cerr << "[LobbyAppState] Error: Client not in lobby status" << std::endl;
        return AppStateCode::MAIN_MENU;
    }

    if (client->getStatus() == InGame) {
        std::cout << "[LobbyAppState] Game started, transitioning to game match state" << std::endl;
        return AppStateCode::GAME_MATCH;
    }

    LobbyWindow lobbyWindow(client);
    int result = lobbyWindow.exec();

    if (result == LobbyWindow::StartGame) {
        if (client->isCreator()) {
            client->startMatch();
            if (client->getStatus() == InGame) {
                return AppStateCode::GAME_MATCH;
            }
        }
    } else if (result == LobbyWindow::LeaveGame) {
        client->leaveMatch();
        return AppStateCode::MAIN_MENU;
    }

    if (client->getStatus() == InGame) {
        return AppStateCode::GAME_MATCH;
    }

    return std::nullopt;
}

LobbyAppState::~LobbyAppState() {
    g_client = nullptr;
}