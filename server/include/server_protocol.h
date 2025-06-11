#ifndef SERVER_PROTOCOL_H_
#define SERVER_PROTOCOL_H_

#include <list>
#include <string>
#include <vector>

#include "../../common/game_info/game_info.h"
#include "../../common/protocol.h"
#include "../../common/socket.h"


class ServerProtocol: public Protocol_ {

public:
    explicit ServerProtocol(Socket&& socketClient);

    void sendConfirmation(bool ok);

    // Envía los nombres de los escenarios disponibles.
    // Se envía luego de aceptar al cliente.
    // void sendScenariesNames(std::vector<std::string> ScenariesNames);


    // se envía una vez comenzada la partida. (Luego de recibir StartMatch)
    void sendMatchInfo(const MatchInfo& matchInfo);

    void sendListOfMatchs(std::list<std::string> matchs_names);

    void sendMatchRoomInfo(const MatchRoomInfo& matchRoomInfo);

    void sendGameInfo(const GameInfo& gameInfo);

    std::string recvUsername();

    MenuAction recvMenuAction();
    LobbyAction recvLobbyAction();
    GameAction recvGameAction();
};

#endif  // SERVER_PROTOCOL_H_
