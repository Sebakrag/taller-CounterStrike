#ifndef CLIENT_PROTOCOL_H_
#define CLIENT_PROTOCOL_H_

#include <list>
#include <string>
#include <vector>

#include "../../common/game_info/game_info.h"
#include "../../common/protocol.h"
#include "../../common/socket.h"


class ClientProtocol: public Protocol_ {
private:
    std::string username;

public:
    ClientProtocol(const std::string& hostname, const std::string& servame,
                   const std::string& username);

    void sendUserName(const std::string& username);
    /**
     * Se utiliza para enviar los mensajes de:
     * - Crear una partida.
     * - Unirse a una partida.
     * - Pedir la lista de partidas.
     * - Salir del juego.
     *
     * En caso de enviar 'crear' o 'unirse', el server enviará una confirmación.
     * Si está OK, también enviará el Tilemap.
     * En caso de enviar 'listar', el server enviará la lista de partidas.
     *
     * Si se intenta enviar uno de estos mensajes cuando el cliente
     * no esté en el menú, el server simplemente los ignorará.
     */
    void sendMenuAction(const MenuAction& action);

    /**
     * Se utiliza para enviar los mensajes de:
     * - Comenzar partida
     * - Abandonar partida
     *
     * En caso de enviar 'comenzar', el server enviará una confirmación.
     * Solo el creador deberá poder comenzar una partida.
     *
     * Si se intenta enviar uno de estos mensajes cuando el cliente
     * no esté en el lobby, el server simplemente los ignorará.
     */
    void sendLobbyAction(const LobbyAction& action);

    /**
     * Envía las acciones que quiere realizar el jugador en el juego.
     * (comprar arma, moverse, disparar, etc.).
     *
     * Si se intenta enviar uno de estos mensajes cuando el cliente
     * no esté en el juego, el server simplemente los ignorará.
     */
    void sendGameAction(const GameAction& gameAction);

    /**
     * Recibe una confirmación del server.
     * Retorna true si es OK o false si es FAIL
     */
    bool recvConfirmation();

    std::vector<std::string> recvListMatchs();


    // recibe la lista de jugadores en la sala
    // y si la partida comenzó
    MatchRoomInfo recvUpdateMatchRoom();

    GameInfo recvGameInfo();
};

#endif  // CLIENT_PROTOCOL_H_
