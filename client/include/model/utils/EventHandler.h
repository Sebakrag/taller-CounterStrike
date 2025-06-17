#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <SDL2/SDL_stdinc.h>

#include "../../../../client/include/client.h"
#include "../../../../client/include/model/World.h"

class EventHandler {
private:
    Client& client;
    World& world;
    Uint32 lastMouseProcessTime = 0;  // variable para contar tiempo

    void handleKeyboardEvents(bool& gameIsRunning) const;
    void handleMouseEvents(bool gameIsRunning);

public:
    EventHandler(Client& client, World& world);

    void handleEvents(bool& gameIsRunning);
};

#endif  // EVENTHANDLER_H
