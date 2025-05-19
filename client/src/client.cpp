#include "../include/client.h"

#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>

#include "../include/client.h"
#include "../include/client_protocol.h"

Client::Client(const std::string& ip, const std::string& port, const std::string& user_name):
        protocol(ip.c_str(), port.c_str(), user_name) {
    protocol.sendUserName(user_name);
    bool ok = protocol.recvConfirmation();
    if (!ok) {
        throw std::runtime_error(
                "No se pudo ingresar. Posiblemente el nombre de usuario ya está en uso.");
    }
    std::cout << "Bienvenido " << user_name << ". Ya estás conectado al servidor." << std::endl;
    status = InMenu;
    // recvDatos();
}

void Client::mainLoop() {
    while (status != Disconnected) {
        if (status == InMenu) {
            menuLoop();
        }
        if (status == InLobby) {
            lobbyLoop();
        }
        if (status == InGame) {
            //   gameLoop();
        }
    }
}


// Para probar (por ahora), voy a leer de la terminal el stdin
void Client::menuLoop() {
    std::cout << "Estás en el Menú" << std::endl;
    std::cout << "Podes escribir: 'crear <nombre>', 'unirse <nombre> ', 'listar' o  'salir'"
              << std::endl;
    std::string msj_usuario;
    while (status == InMenu) {
        std::getline(std::cin, msj_usuario);
        if (msj_usuario.substr(0, 6) == "crear ") {
            std::string nombre_partida = msj_usuario.substr(6, msj_usuario.length());
            CreateMatch(nombre_partida);
        } else if (msj_usuario.substr(0, 7) == "unirse ") {
            std::string nombre_partida = msj_usuario.substr(7, msj_usuario.length());
            JoinMatch(nombre_partida);
        } else if (msj_usuario == "listar") {
            refreshMatchList();
        } else if (msj_usuario == "salir") {
            ExitGame();
        }
    }
}

// Para probar (por ahora), voy a leer de la terminal el stdin
void Client::lobbyLoop() {
    std::cout << "Estás en el Lobby" << std::endl;
    std::cout << "Podes escribir: 'actualizar', 'abandonar', 'iniciar'" << std::endl;

    std::string msj_usuario;
    while (status == InLobby) {
        std::getline(std::cin, msj_usuario);
        if (msj_usuario == "actualizar") {
            refreshPlayerList();
        } else if (msj_usuario == "abandonar") {
            LeaveMatch();
        } else if (msj_usuario == "iniciar") {
            StartMatch();
        }
    }
}

void Client::ExitGame() {
    status = Disconnected;
    protocol.sendMenuAction(MenuAction(MenuActionType::Exit));
}

void Client::CreateMatch(const std::string& match_name) {
    protocol.sendMenuAction(MenuAction(MenuActionType::Create, match_name, 0));
    bool created = protocol.recvConfirmation();
    if (created) {
        std::cout << "La partida se creó correctamente." << std::endl;
        status = InLobby;
        // protocol.recvTileMap
    } else {
        std::cout << "La partida No se pudo crear." << std::endl;
    }
}

void Client::JoinMatch(const std::string& match_name) {
    protocol.sendMenuAction(MenuAction(MenuActionType::Join, match_name));
    bool united = protocol.recvConfirmation();
    if (united) {
        std::cout << "Te uniste a la partida!" << std::endl;
        status = InLobby;
        // protocol.recvTileMap
    } else {
        std::cout << "No pudiste unirte a la partida." << std::endl;
    }
}

void Client::refreshMatchList() {
    protocol.sendMenuAction(MenuAction(MenuActionType::List));
    std::vector<std::string> lista_partidas = protocol.recvListMatchs();

    std::cout << "Partidas:" << std::endl;
    for (const auto& partida: lista_partidas) {
        std::cout << " - " << partida << std::endl;
    }
}

// in Lobby.
void Client::LeaveMatch() {
    protocol.sendLobbyAction(LobbyAction::QuitMatch);
    status = InMenu;
}

void Client::StartMatch() {
    protocol.sendLobbyAction(LobbyAction::StartMatch);
    bool ok = protocol.recvConfirmation();
    if (ok) {
        std::cout << "Empezó la partida" << std::endl;
    } else {
        std::cout << "No empezó la partida. No sos el anfitrion." << std::endl;
    }
    // YOUR CODE
}

void Client::refreshPlayerList() {
    protocol.sendLobbyAction(LobbyAction::ListPlayers);
    std::vector<PlayerInfoLobby> players = protocol.recvListPlayers();

    std::cout << "Players en la sala: " << std::endl;
    for (const auto& p: players) {
        std::cout << " - " << p.username << std::endl;
    }
}


// in Game.
void Client::BuyWeapon() {
    // YOUR CODE
}

void Client::buyAmmo() {
    // YOUR CODE
}

void Client::Walk() {
    // YOUR CODE
}

void Client::Attack() {
    // YOUR CODE
}

void Client::SwitchWeapon() {
    // YOUR CODE
}

void Client::PickUp() {
    // YOUR CODE
}

void Client::refreshGameState() {
    // YOUR CODE
}
