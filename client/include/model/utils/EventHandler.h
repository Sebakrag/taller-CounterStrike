#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <SDL2/SDL_stdinc.h>

#include "../../../../client/include/client.h"
#include "../../../../client/include/model/World.h"

class EventHandler {
private:
    const Uint32 DELAY_KEYBOARD_PRESS = 0;  // Ajustá este valor (en milisegundos)
    const Uint32 DELAY_MOUSE_MOTION = 150;  // Ajustá este valor (en milisegundos)

    Client& client;
    World& world;
    Uint32 lastKeyboardPress = 0;     // variable para contar tiempo
    Uint32 lastMouseProcessTime = 0;  // variable para contar tiempo
    float lastAimAngle = 0;           // para no enviar mensajes al pe

    void handleKeyboardEvents(bool& gameIsRunning);
    void handleMouseEvents(bool gameIsRunning);

    // para deteccion de teclas de forma discreta (sin mantener apretado)
    void handleKeyDown(SDL_Scancode sc, bool& gameIsRunning) const;
    // para deteccion de boton apretado de forma discreta (sin mantener apretado)
    void handleMouseButtonDown(const SDL_MouseButtonEvent& b);

public:
    EventHandler(Client& client, World& world);

    void handleEvents(bool& gameIsRunning);
};

#endif  // EVENTHANDLER_H
