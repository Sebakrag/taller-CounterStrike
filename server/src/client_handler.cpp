#include "../include/client_handler.h"

#include <iostream>
#include <list>
#include <string>
#include <utility>

ClientHandler::ClientHandler(ServerProtocol&& serverProtocol, const std::string& username,
                             GameManager& gameManager):
        username(username),
        protocol(std::move(serverProtocol)),
        status(InMenu),
        gameManager(gameManager),
        receiver(username, protocol, queueActionsPlayers) {
    protocol.sendInitMsg();
    start();
}

void ClientHandler::run() {
    try {
        while (should_keep_running() && status != Disconnected) {
            while (status == InMenu) {
                MenuAction menuAction = protocol.recvMenuAction();
                handleMenuActions(menuAction);
            }
            while (status == InLobby) {
                LobbyAction lobbyAction = protocol.recvLobbyAction();
                handleLobbyActions(lobbyAction);
            }
            if (status == InGame) {
                receiver.start();
                receiver.join();
                // YOUR CODE
                // algo asi:
                // sender y receiver start
                //  match.start();
                // sender y receiver join()
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "ClientHandler error: " << e.what() << std::endl;
    }
}

void ClientHandler::kill() {
    protocol.shutDown(2);
    status = Disconnected;
    stop();
}

std::string ClientHandler::getUsername() { return username; }

void ClientHandler::handleMenuActions(const MenuAction& menuAction) {
    bool aux = false;
    switch (menuAction.type) {
        case MenuActionType::Create:
            aux = gameManager.createMatch(menuAction.name_match, username);
            protocol.sendConfirmation(aux);
            break;
        case MenuActionType::Join:
            aux = gameManager.JoinMatch(menuAction.name_match, username);
            protocol.sendConfirmation(aux);
            break;
        case MenuActionType::List: {
            std::list<std::string> matchs_list = gameManager.listMatchs();
            std::string msg;
            for (const auto& match: matchs_list) {
                msg += match + "\n";
            }
            protocol.sendMessage(TypeMessage::ListMatchs, msg);
            break;
        }
        case MenuActionType::Exit:
            status = Disconnected;
            break;
        default:
            break;
    }
    if (aux) {
        std::cout << username << " entro al lobby" << std::endl;
        status = InLobby;
        myMatch = menuAction.name_match;
        //...protocol.sendTilemap(...);
    }
}

void ClientHandler::handleLobbyActions(const LobbyAction& lobbyAction) {
    switch (lobbyAction) {
        case LobbyAction::QuitMatch:
            gameManager.QuitMatch(myMatch, username);
            myMatch = "";
            status = InMenu;
            break;
        case LobbyAction::StartMatch: {
            bool ok = gameManager.startMatch(username, myMatch);
            if (ok) {
                status = InGame;
            }
            protocol.sendConfirmation(ok);
            break;
        }
        case LobbyAction::ListPlayers:
            std::vector<PlayerInfoLobby> playersInfo = gameManager.getPlayersInMatch(myMatch);
            protocol.sendListPlayers(playersInfo);
            break;
    }
}
