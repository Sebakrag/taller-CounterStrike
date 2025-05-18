#include "client_handler.h"

#include <iostream>
#include <list>
#include <string>
#include <utility>

#include "server_protocol.h"

// ClientHandler::ClientHandler(Socket peer)
//     : peer(std::move(peer)){}


ClientHandler::ClientHandler(ServerProtocol&& serverProtocol, const std::string& username,
                             GameManager& gameManager):
        username(username),
        protocol(std::move(serverProtocol)),
        status(InMenu),
        gameManager(gameManager) {
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
            if (aux)
                isHostMatch = true;
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
            if (isHostMatch) {
                // elegir otro anfitrion o destruir partida y mandar a todos al menu.
            }
            break;
        default:
            break;
    }
    if (aux) {
        std::cout << username << " entro al lobby" << std::endl;
        status = InLobby;
        myMatch = gameManager.getMatch(menuAction.name_match, username);
        //...protocol.sendTilemap(...);
    }
}

void ClientHandler::handleLobbyActions(const LobbyAction& lobbyAction) {
    switch (lobbyAction) {
        case LobbyAction::QuitMatch:
            status = InMenu;
            break;
        case LobbyAction::StartMatch:
            if (isHostMatch) {
                status = InGame;
                protocol.sendConfirmation(true);
            } else {
                protocol.sendConfirmation(false);
            }
            break;
        case LobbyAction::ListPlayers:
            std::vector<std::string> playersInMatch = myMatch->getPlayers();
            std::vector<PlayerInfoLobby> playersInfo;
            for (const auto& player: playersInMatch) {
                playersInfo.push_back(PlayerInfoLobby(player, Team::Terrorist));
            }
            protocol.sendListPlayers(playersInfo);
            break;
    }
}
