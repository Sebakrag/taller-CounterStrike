#include "client/include/app-state/LobbyAppState.h"

#include <iostream>
#include <optional>

#include "client/include/app-state/AppStateController.h"
#include "client/include/client.h"
#include "client/include/ui/LobbyWindow.h"

// Referencia a la variable global definida en MainMenuWindow.cpp
extern Client* g_client;

LobbyAppState::LobbyAppState(AppStateController* ctrl) { controller = ctrl; }

std::optional<AppStateCode> LobbyAppState::update() {
    Client* client = controller->getClient();
    if (!client) {
        std::cerr << "[LobbyAppState] ERROR: No client available" << std::endl;
        return AppStateCode::MAIN_MENU;
    }

    // Actualizar la variable global para que los componentes UI puedan acceder al cliente
    g_client = client;

    // Verificar si el cliente está en el lobby
    if (client->getStatus() != InLobby && client->getStatus() != InGame) {
        std::cerr << "[LobbyAppState] ERROR: Client not in lobby status" << std::endl;
        return AppStateCode::MAIN_MENU;
    }

    // Si el juego ya comenzó, transicionar al estado de juego
    if (client->getStatus() == InGame) {
        return AppStateCode::GAME_MATCH;
    }

    // Mostrar la ventana de lobby
    LobbyWindow lobbyWindow(client);
    int result = lobbyWindow.exec();

    if (result == LobbyWindow::StartGame) {
        if (client->isCreator()) {
            client->StartMatch();
            if (client->getStatus() == InGame) {
                return AppStateCode::GAME_MATCH;
            }
        }
    } else if (result == LobbyWindow::LeaveGame) {
        client->LeaveMatch();
        return AppStateCode::MAIN_MENU;
    }

    if (client->getStatus() == InGame) {
        return AppStateCode::GAME_MATCH;
    }

    return std::nullopt;
}

LobbyAppState::~LobbyAppState() {}
