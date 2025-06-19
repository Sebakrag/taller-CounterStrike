#include "../include/client_handler.h"

#include <iostream>
#include <list>
#include <ostream>
#include <string>
#include <utility>

ClientHandler::ClientHandler(ServerProtocol&& serverProtocol, const std::string& username,
                             GameManager& gameManager):
        username(username),
        protocol(std::move(serverProtocol)),
        status(InMenu),
        gameManager(gameManager),
        senderQueue(std::make_shared<Queue<GameInfo>>()),
        sender(protocol, senderQueue) {
    // protocol.sendInitMsg();
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
                receiver->start();
                sender.start();

                if (status != Disconnected) {
                    receiver->join();
                    std::cout << "reciever joineado" << std::endl;
                    // al finalizar, regresar al menú o cambiar a disconected
                    try {
                        senderQueue->close();
                    } catch (...) {}
                    sender.join();
                    std::cout << "sender joineado" << std::endl;

                    status = InMenu;
                    // delete receiver;
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "ClientHandler: " << e.what() << std::endl;
    }
}

void ClientHandler::kill() {
    protocol.shutDown(2);
    if (status == InGame) {
        std::cout << "KILL client handler" << std::endl;
        status = Disconnected;
        sender.kill();
        receiver->kill();
        // sender.join();
        // receiver->join();
        // delete receiver;
    }
    // status = Disconnected;
    stop();
}

std::string ClientHandler::getUsername() { return username; }

void ClientHandler::handleMenuActions(const MenuAction& menuAction) {
    bool aux = false;
    switch (menuAction.type) {
        case MenuActionType::Create:
            aux = gameManager.createMatch(menuAction.name_match, username, senderQueue, "demo");
            protocol.sendConfirmation(aux);
            break;
        case MenuActionType::Join:
            aux = gameManager.JoinMatch(menuAction.name_match, username, senderQueue);
            protocol.sendConfirmation(aux);
            break;
        case MenuActionType::List: {
            std::list<std::string> matchs_list = gameManager.listMatchs();
            protocol.sendListOfMatchs(matchs_list);
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
        // MatchInfo matchInfo = gameManager.getMatchInfo(myMatch);
        // matchInfo.print();
        //
        // protocol.sendMatchInfo(matchInfo);
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
            bool ok = gameManager.StartMatch(username, myMatch);
            protocol.sendConfirmation(ok);
            if (ok) {
                status = InGame;
                MatchInfo matchInfo = gameManager.getMatchInfo(myMatch, username);
                protocol.sendMatchInfo(matchInfo);
                receiver = new Receiver(username, protocol, gameManager.getActionsQueue(myMatch));
            }
            break;
        }
        case LobbyAction::ListPlayers:
            auto info = gameManager.getMatchRoomInfo(myMatch);
            protocol.sendMatchRoomInfo(info);
            if (info.matchStarted) {
                status = InGame;
                MatchInfo matchInfo = gameManager.getMatchInfo(myMatch, username);
                protocol.sendMatchInfo(matchInfo);
                receiver = new Receiver(username, protocol, gameManager.getActionsQueue(myMatch));
            }
            break;
    }
}

ClientHandler::~ClientHandler() {
    if (receiver) {
        delete receiver;
    }
}
