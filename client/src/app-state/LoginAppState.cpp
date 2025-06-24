#include "../../../client/include/app-state/LoginAppState.h"

#include <iostream>
#include <memory>
#include <string>

// Qt includes
#include <QDialog>
#include <QMessageBox>

#include "client/include/app-state/AppStateController.h"
#include "ui/LoginWindow.h"
#include "ui/UIConstants.h"

#include "client.h"

LoginAppState::LoginAppState(AppStateController* ctrl) {
    controller = ctrl;
    // Here we can initialize some music. This applies only if we
    // are using the heap to create the app states.
    // Otherwise, we should override the enter() method of AppState, and
    // there we should initialize the music.
}

std::optional<AppStateCode> LoginAppState::update() {
    LoginWindow dlg;
    if (dlg.exec() == QDialog::Accepted) {
        const std::string usr = dlg.userName().toStdString();
        const std::string ip = dlg.serverIp().toStdString();
        const std::string port = dlg.serverPort().toStdString();


        try {
            auto c = std::make_unique<Client>(ip, port, usr);

            controller->setClient(std::move(c));

            AppStateCode next_state = AppStateCode::MAIN_MENU;
            return next_state;
        } catch (const std::exception& e) {
            std::cerr << "[LoginAppState] ERROR: Client not in lobby status" << e.what()
                      << std::endl;
            return AppStateCode::LOGIN;
        }
    }
    return AppStateCode::LOGIN;
}

LoginAppState::~LoginAppState() {}
