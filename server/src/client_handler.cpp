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
            // Enviar la lista de mapas disponibles al cliente
            protocol.sendAvailableMaps(gameManager.getAvailableMaps());
            
            // Crear la partida con el mapa seleccionado
            aux = gameManager.createMatch(menuAction.name_match, username, senderQueue, 
                                         menuAction.map_file_name);
            protocol.sendConfirmation(aux);
            break;
        case MenuActionType::Join:
            aux = gameManager.JoinMatch(menuAction.name_match, username, senderQueue);
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
        
        // Guardar el nombre del archivo del mapa seleccionado si estamos creando una partida
        if (menuAction.type == MenuActionType::Create && !menuAction.map_file_name.empty()) {
            std::cout << "Mapa seleccionado: " << menuAction.map_file_name << std::endl;
            // En una implementación completa, aquí guardaríamos el nombre del mapa para usarlo al iniciar la partida
            // Por ahora lo manejamos en el StartMatch
        }
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
            if (ok) {
                status = InGame;
                receiver = new Receiver(username, protocol, gameManager.getActionsQueue(myMatch));
            }
            protocol.sendConfirmation(ok);
            break;
        }
        case LobbyAction::ListPlayers:
            auto info = gameManager.getMatchRoomInfo(myMatch);
            if (info.matchStarted) {
                status = InGame;
                receiver = new Receiver(username, protocol, gameManager.getActionsQueue(myMatch));
            }
            protocol.sendMatchRoomInfo(info);
            break;
    }
}
ClientHandler::~ClientHandler() {
    if (receiver) {
        delete receiver;
    }
}
