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

    void mainLoop();
    void menuLoop();
    void lobbyLoop();

    // TO DO: traducir los nombres de las funciones

    void SalirDelJuego();
    void CrearPartida(const std::string& nombre_partida);
    void UnirseAPartida(const std::string& nombre_partida);
    void refrescarListaDePartidas();

    void AbandonarPartida();  // regresa al menú
    void IniciarPartida();
    void refrescarListaJugadores();

    void ComprarArma();
    void comprarMuniciones();
    void Caminar();
    void Atacar();
    void CambiarArma();
    void Agarrar();

    void refrescarEstadoJuego();
};

#endif  // CLIENT_H_
