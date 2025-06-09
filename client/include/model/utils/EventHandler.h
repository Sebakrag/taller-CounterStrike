#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <SDL_stdinc.h>

#include "../../../../client/include/client.h"
#include "../../../../client/include/model/World.h"

class EventHandler {
private:
    Client* client;
    // Una vez que tengamos la conexion con el servidor a traves del client, el world no creo
    // que lo necesitemos aca. (O si lo necesito para modificar la direccion a la que apunta?)
    // No, me parece que estaria bueno que la rotacion de la camara tambien se envie al server
    // para que la vision del jugador se informe a todos los jugadores al mismo tiempo (inclusive
    // para el jugador local). Es decir, es una forma de sincronizar la vision entre todos los
    // jugadores en cada frame.
    World& world;
    Uint32 lastMouseProcessTime = 0;  // variable para contar tiempo

    void handleKeyboardEvents(bool& gameIsRunning);
    void handleMouseEvents(bool gameIsRunning);

public:
    EventHandler(Client* client, World& world);

    void handleEvents(bool& gameIsRunning);
};

#endif  // EVENTHANDLER_H
