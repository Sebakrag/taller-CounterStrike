#ifndef CLIENT_H_
#define CLIENT_H_

#include <string>
#include <vector>

#include "../../common/queue.h"

#include "client_protocol.h"
#include "client_receiver.h"
#include "client_sender.h"

enum Status { Disconnected, InMenu, InLobby, InGame };

// Clase para probar la conexion. Determinar despues si sirve o no
class Client {
private:
    ClientProtocol protocol;
    Status status;
    std::string match_name;
    bool player_creator = false;
    Queue<GameAction> send_queue;
    Queue<GameInfo> recv_queue;
    ClientSender sender;
    ClientReceiver receiver;

public:
    Client(const std::string& ip, const std::string& port, const std::string& user_name);

    // estos loop NO IRÍAN ACÁ.
    void mainLoop();
    void menuLoop();
    void lobbyLoop();

    // Acciones en el menu principal
    void ExitGame();
    void CreateMatch(const std::string& match_name);
    void JoinMatch(const std::string& match_name);
    void refreshMatchList();

    // Acciones en el lobby (antes de empezar la partida)
    void LeaveMatch();  // regresa la menú principal
    void StartMatch();
    void refreshPlayersList();

    ~Client();

    Status getStatus();
    Queue<GameAction>& getSendeQueue();
    Queue<GameInfo>& getReceiverQueue();
};

#endif  // CLIENT_H_
