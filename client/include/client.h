#ifndef CLIENT_H_
#define CLIENT_H_

#include <string>
#include <utility>
#include <vector>

#include "client/dtos/AimInfo.h"
#include "common/queue.h"

#include "client_protocol.h"
#include "client_receiver.h"
#include "client_sender.h"

enum Status { Disconnected, InMenu, InLobby, InGame };

class Client {
private:
    ClientProtocol protocol;
    Status status;
    std::string username;
    std::string match_name;  // TODO: eliminar este miembro. (Ya esta dentro de matchInfo)
    bool player_creator = false;
    Queue<GameAction> send_queue;
    Queue<GameInfo> recv_queue;
    ClientSender sender;
    ClientReceiver receiver;
    MatchInfo matchInfo;

public:
    Client(const std::string& ip, const std::string& port, const std::string& user_name);

    // Acciones en el menu principal
    void ExitGame();
    bool CreateMatch(const std::string& match_name);
    void JoinMatch(const std::string& match_name);
    std::vector<std::string> refreshMatchList();

    // Acciones en el lobby (antes de empezar la partida)
    void LeaveMatch();  // regresa la menú principal
    void StartMatch();
    std::vector<PlayerInfoLobby> refreshMatchRoom();
    std::vector<PlayerInfoLobby> refreshPlayersList();
    std::string getUsername() const { return username; }
    bool isCreator() const { return player_creator; }

    MatchInfo getMatchInfo();

    GameInfo getGameInfo();     // sync
    GameInfo tryGetGameInfo();  // Async

    void move(const Vec2D& direction);
    void shoot(const AimInfo& aimInfo);
    void rotate(float angle);
    void pickUpItem(const Vec2D& playerPosition);

    ~Client();

    Status getStatus();
};

#endif  // CLIENT_H_
