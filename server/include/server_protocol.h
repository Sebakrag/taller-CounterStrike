#ifndef SERVER_PROTOCOL_H_
#define SERVER_PROTOCOL_H_

#include <string>
#include <vector>

#include "../../common/game_info/game_info.h"
#include "../../common/protocol.h"
#include "../../common/socket.h"
#include "map_manager.h"

enum TypeMessage { ListMatchs, ListPlayers, ListMaps };

class ServerProtocol: public Protocol_ {
public:
    explicit ServerProtocol(Socket&& socketClient);

    void sendConfirmation(bool ok);

    // envía la lista de escenarios.
    void sendInitMsg();

    // Cuando comience una partida,
    // se deberá enviar el tilemap del escenario seleccionado.
    // void sendTileMap(TileMap);

    void sendMessage(TypeMessage typeMenssage, const std::string& msg);

    void sendMatchRoomInfo(const MatchRoomInfo& matchRoomInfo);

    void sendGameInfo(const GameInfo& gameInfo);

    void sendAvailableMaps(const std::vector<MapInfo>& mapList);

    std::string recvUsername();

    MenuAction recvMenuAction();
    LobbyAction recvLobbyAction();
    GameAction recvGameAction();
};

#endif  // SERVER_PROTOCOL_H_
