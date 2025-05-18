#ifndef CLIENT_H_
#define CLIENT_H_

#include <string>
#include <vector>

#include "client_protocol.h"

enum Status { Disconnected, InMenu, InLobby, InGame };

// Clase para probar la conexion. Determinar despues si sirve o no
class Client {
    ClientProtocol protocol;
    Status status;
    std::string match_name;
    bool player_creator = false;

public:
    Client(const std::string& ip, const std::string& port, const std::string& user_name);

    // estos loop NO IRÍAN.
    void mainLoop();
    void menuLoop();
    void lobbyLoop();

    void ExitGame();
    void CreateMatch(const std::string& match_name);
    void JoinMatch(const std::string& match_name);
    void refreshMatchList();

    void LeaveMatch();  // returns to menu
    void StartMatch();
    void refreshPlayerList();

    void BuyWeapon();
    void buyAmmo();
    void Walk();
    void Attack();
    void SwitchWeapon();
    void PickUp();

    void refreshGameState();
};

#endif  // CLIENT_H_
