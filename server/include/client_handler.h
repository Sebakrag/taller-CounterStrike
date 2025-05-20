#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../../common/queue.h"
#include "../../common/socket.h"
#include "../../common/thread.h"

#include "game_manager.h"
#include "receiver.h"
#include "server_protocol.h"

enum Status { Disconnected, InMenu, InLobby, InGame };

class ClientHandler: public Thread {
private:
    std::string username;
    ServerProtocol protocol;
    Status status;
    std::string myMatch;
    GameManager& gameManager;
    Queue<GameInfo> senderQueue;
    Queue<PlayerAction> queueActionsPlayers;  // Esto no iría acá
    Receiver receiver;

public:
    ClientHandler(ServerProtocol&& serverProtocol, const std::string& username,
                  GameManager& gameManager);

    void run() override;

    void kill();

    std::string getUsername();

private:
    void handleMenuActions(const MenuAction& menuAction);
    void handleLobbyActions(const LobbyAction& lobbyAction);
};
