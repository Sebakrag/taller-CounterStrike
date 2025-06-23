#include "client/include/app-state/MainMenuAppState.h"

#include <iostream>
#include <optional>

#include "client/include/app-state/AppStateCode.h"
#include "client/include/app-state/AppStateController.h"
#include "client/include/client.h"
#include "client/include/ui/JoinGameDialog.h"
#include "client/include/ui/MainMenuWindow.h"
#include "client/include/ui/NameInputDialog.h"
#include "client/include/ui/StyledMessageBox.h"

extern Client* g_client;

MainMenuAppState::MainMenuAppState(AppStateController* ctrl) {
    controller = ctrl;
    std::cout << "[MainMenuAppState] Constructor called" << std::endl;
    // Initialize g_client to nullptr to avoid accessing uninitialized memory
    g_client = nullptr;
}

std::optional<AppStateCode> MainMenuAppState::update() {
    // Get client pointer and check if it's valid
    Client* client_ptr = controller->getClient();
    if (client_ptr == nullptr) {
        std::cerr << "[MainMenuAppState] Error: Client pointer is null" << std::endl;
        return AppStateCode::QUIT;
    }
    g_client = client_ptr;

    MainMenuWindow menu_window;
    int result = menu_window.exec();

    switch (result) {
        case MainMenuWindow::CreateGame: {
            NameInputDialog nid("Crear Partida", "Ingrese nombre de la nueva partida");
            if (nid.exec() != QDialog::Accepted || !nid.accepted()) {
                return AppStateCode::MAIN_MENU;
            }
            QString gameName = nid.textValue().trimmed();
            bool success = controller->getClient()->CreateMatch(gameName.toStdString(), "escenario1");

            if (!success) {
                showStyledWarning("Error", "No se pudo crear la partida. Es posible que ya exista "
                                           "una partida con ese nombre.");
                return AppStateCode::MAIN_MENU;
            }
            break;
        }
        case MainMenuWindow::JoinGame: {
            JoinGameDialog joinDialog;
            int result = joinDialog.exec();

            if (result != JoinGameDialog::JoinSelected) {
                return AppStateCode::MAIN_MENU;
            }
            break;
        }
        case MainMenuWindow::QuitApp:
            controller->getClient()->ExitGame();
            return AppStateCode::QUIT;
        default:
            return AppStateCode::QUIT;
    }

    if (controller->getClient()->getStatus() == InLobby) {
        return AppStateCode::LOBBY;
    }
    return AppStateCode::MAIN_MENU;
}

MainMenuAppState::~MainMenuAppState() {}
