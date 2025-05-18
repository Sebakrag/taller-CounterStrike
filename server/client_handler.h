#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../common/socket.h"
#include "../common/thread.h"

#include "game_manager.h"
#include "server_protocol.h"

enum Status { Disconnected, InMenu, InLobby, InGame };

class ClientHandler: public Thread {
private:
    std::string username;
    ServerProtocol protocol;
    Status status;
    bool isHostMatch = false;
    std::shared_ptr<Match> myMatch;
    GameManager& gameManager;

public:
    // ClientHandler(Socket peer);
    ClientHandler(ServerProtocol&& serverProtocol, const std::string& username,
                  GameManager& gameManager);

    void run() override;

    void kill();

    std::string getUsername();

private:
    void handleMenuActions(const MenuAction& menuAction);
    void handleLobbyActions(const LobbyAction& lobbyAction);
};
