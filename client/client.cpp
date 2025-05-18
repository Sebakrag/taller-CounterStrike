#include "client.h"

#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>

#include "client_protocol.h"

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
}

void Client::mainLoop() {
    while (status != Disconnected) {
        if (status == InMenu) {
            menuLoop();
        }
        if (status == InLobby) {
            lobbyLoop();
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
            CrearPartida(nombre_partida);
        } else if (msj_usuario.substr(0, 7) == "unirse ") {
            std::string nombre_partida = msj_usuario.substr(7, msj_usuario.length());
            UnirseAPartida(nombre_partida);
        } else if (msj_usuario == "listar") {
            refrescarListaDePartidas();
        } else if (msj_usuario == "salir") {
            SalirDelJuego();
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
            refrescarListaJugadores();
        } else if (msj_usuario == "abandonar") {
            AbandonarPartida();
        } else if (msj_usuario == "iniciar") {
            IniciarPartida();
        }
    }
}

void Client::SalirDelJuego() {
    status = Disconnected;
    protocol.sendMenuAction(MenuAction(MenuActionType::Exit));
}

void Client::CrearPartida(const std::string& nombre_partida) {
    protocol.sendMenuAction(MenuAction(MenuActionType::Create, nombre_partida, 0));
    bool created = protocol.recvConfirmation();
    if (created) {
        std::cout << "La partida se creó correctamente." << std::endl;
        status = InLobby;
        // protocol.recvTileMap
    } else {
        std::cout << "La partida No se pudo crear." << std::endl;
    }
}

void Client::UnirseAPartida(const std::string& nombre_partida) {
    protocol.sendMenuAction(MenuAction(MenuActionType::Join, nombre_partida));
    bool united = protocol.recvConfirmation();
    if (united) {
        std::cout << "Te uniste a la partida!" << std::endl;
        status = InLobby;
        // protocol.recvTileMap
    } else {
        std::cout << "No pudiste unirte a la partida." << std::endl;
    }
}

void Client::refrescarListaDePartidas() {
    protocol.sendMenuAction(MenuAction(MenuActionType::List));
    std::vector<std::string> lista_partidas = protocol.recvListMatchs();

    std::cout << "Partidas:" << std::endl;
    for (const auto& partida: lista_partidas) {
        std::cout << " - " << partida << std::endl;
    }
}

// in Lobby.
void Client::AbandonarPartida() {
    protocol.sendLobbyAction(LobbyAction::QuitMatch);
    status = InMenu;
}

void Client::IniciarPartida() {
    std::cerr << "Todavia no se puede. " << std::endl;
    // YOUR CODE
}

void Client::refrescarListaJugadores() {
    protocol.sendLobbyAction(LobbyAction::ListPlayers);
    std::vector<PlayerInfoLobby> players = protocol.recvListPlayers();

    std::cout << "Players en la sala: " << std::endl;
    for (auto& jugador: players) {
        std::cout << " - " << jugador.username << std::endl;
    }
}


// in Game.
void Client::ComprarArma() {
    // YOUR CODE
}

void Client::comprarMuniciones() {
    // YOUR CODE
}

void Client::Caminar() {
    // YOUR CODE
}

void Client::Atacar() {
    // YOUR CODE
}

void Client::CambiarArma() {
    // YOUR CODE
}

void Client::Agarrar() {
    // YOUR CODE
}

void Client::refrescarEstadoJuego() {
    // YOUR CODE
}
