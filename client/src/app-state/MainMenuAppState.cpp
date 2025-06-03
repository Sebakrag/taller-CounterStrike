#include "app-state/MainMenuAppState.h"

#include <iostream>
#include <optional>

#include "app-state/AppStateCode.h"
#include "app-state/AppStateController.h"
#include "client.h"
#include "ui/MainMenuWindow.h"
#include "ui/JoinGameDialog.h"
#include "ui/NameInputDialog.h"
#include "ui/StyledMessageBox.h"

extern Client* g_client;

#include "ui/NameInputDialog.h"

MainMenuAppState::MainMenuAppState() {
}

std::optional<AppStateCode> MainMenuAppState::update() {
    g_client = controller->getClient();

    MainMenuWindow menu_window;
    int result = menu_window.exec();

    switch (result) {
        case MainMenuWindow::CreateGame: {
            NameInputDialog nid("Crear Partida", "Ingrese nombre de la nueva partida");
            if (nid.exec() != QDialog::Accepted || !nid.accepted()) {
                return AppStateCode::MAIN_MENU;
            }
            QString gameName = nid.textValue().trimmed();
            bool success = controller->getClient()->CreateMatch(gameName.toStdString());

            if (!success) {
                showStyledWarning("Error", "No se pudo crear la partida. Es posible que ya exista una partida con ese nombre.");
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

MainMenuAppState::~MainMenuAppState() {
}