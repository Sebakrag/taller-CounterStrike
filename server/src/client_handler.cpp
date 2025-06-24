#include "../include/client_handler.h"

#include <iostream>
#include <list>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>

#include "../include/scenario_registry.h"

ClientHandler::ClientHandler(ServerProtocol&& serverProtocol, const std::string& username,
                             GameManager& gameManager):
        username(username),
        protocol(std::move(serverProtocol)),
        status(InMenu),
        gameManager(gameManager),
        senderQueue(std::make_shared<Queue<GameInfo>>()) {
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
                if (sender == nullptr || receiver == nullptr) {
                    throw std::runtime_error("Sender o receiver nulos.");
                }
                receiver->start();
                sender->start();
                receiver->join();

                // if (status != Disconnected) {
                //                    receiver->join();
                delete receiver;
                receiver = nullptr;
                // al finalizar, regresar al menú o cambiar a disconected
                if (sender->is_alive()) {
                    try {
                        sender->kill();
                        // senderQueue->close();
                    } catch (...) {}
                }
                sender->join();
                // sender->join();
                delete sender;
                sender = nullptr;
                senderQueue = std::make_shared<Queue<GameInfo>>();  // reseteo la sender queue

                status = InMenu;
                // delete receiver;
                //}
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "ClientHandler: " << e.what() << std::endl;
    }
}

void ClientHandler::kill() {
    try {
        protocol.shutDown(2);
    } catch (...) {}

    status = Disconnected;
    if (sender) {
        sender->kill();
    }
    if (receiver) {
        receiver->stop();
    }
    //        receiver->kill();
    // sender->join();
    // receiver->join();
    //}
    status = Disconnected;
    stop();
}

std::string ClientHandler::getUsername() { return username; }

void ClientHandler::handleMenuActions(const MenuAction& menuAction) {
    bool aux = false;
    switch (menuAction.type) {
        case MenuActionType::Create: {
            std::string selectedScenario = menuAction.scenario_name;

            if (!ScenarioRegistry::existsScenario(selectedScenario)) {
                ScenarioRegistry::loadMapFromYaml(selectedScenario);
            }

            if (ScenarioRegistry::existsScenario(selectedScenario)) {
                aux = gameManager.createMatch(menuAction.name_match, username, senderQueue,
                                              selectedScenario);
            } else {
                aux = gameManager.createMatch(menuAction.name_match, username, senderQueue, "demo");
            }

            protocol.sendConfirmation(aux);
            break;
        }
        case MenuActionType::Join:
            aux = gameManager.JoinMatch(menuAction.name_match, username, senderQueue);
            protocol.sendConfirmation(aux);
            break;
        case MenuActionType::List: {
            std::list<std::string> matchs_list = gameManager.listMatchs();
            protocol.sendListOfMatchs(matchs_list);
            break;
        }
        case MenuActionType::ListScenarios: {
            std::vector<std::string> scenarios = ScenarioRegistry::listAvailableMaps();
            protocol.sendListOfScenarios(scenarios);
            break;
        }
        case MenuActionType::Exit:
            status = Disconnected;
            break;
        default:
            break;
    }
    if (aux) {
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
            protocol.sendConfirmation(true);
            break;
        case LobbyAction::StartMatch: {
            bool ok = gameManager.StartMatch(username, myMatch);
            protocol.sendConfirmation(ok);
            if (ok) {
                status = InGame;
                MatchInfo matchInfo = gameManager.getMatchInfo(myMatch, username);
                protocol.sendMatchInfo(matchInfo);
                receiver = new Receiver(username, protocol, gameManager.getActionsQueue(myMatch));
                // senderQueue = std::make_shared<Queue<GameInfo>>(); //reseteo la sender queue
                sender = new Sender(protocol, senderQueue);
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
                sender = new Sender(protocol, senderQueue);
            }
            break;
    }
}

ClientHandler::~ClientHandler() {
    if (receiver) {
        delete receiver;
    }
    if (sender) {
        delete sender;
    }
}
