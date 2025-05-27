#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "client/include/model/World.h"

class EventHandler {
private:
    // Client& client;
    World& world;  // Por ahora le dejo una referencia al world para poder mover al jugador.
    // Una vez que tengamos la conexion con el servidor a traves del client, el world no creo
    // que lo necesitemos aca. (O si lo necesito para modificar la direccion a la que apunta?)
    void handleKeyboardEvents(bool& gameIsRunning) const;
    // void handleMouseEvents();

public:
    // EventHandler(Client& client, World& world);
    explicit EventHandler(World& world);

    void handleEvents(bool& gameIsRunning) const;
};

#endif  // EVENTHANDLER_H
