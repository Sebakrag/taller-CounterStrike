#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_stdinc.h>

#include "../../../../client/include/client.h"
#include "../../../../client/include/model/World.h"
#include "client/include/model/Shop.h"

class EventHandler {
private:
    const Uint32 DELAY_KEYBOARD_PRESS = 0;  // Ajustá este valor (en milisegundos)
    const Uint32 DELAY_MOUSE_MOTION = 150;  // Ajustá este valor (en milisegundos)

    Client& client;
    World& world;
    Shop& shop;
    Uint32 lastKeyboardPress = 0;     // variable para contar tiempo
    Uint32 lastMouseProcessTime = 0;  // variable para contar tiempo
    float lastAimAngle = 0;           // para no enviar mensajes al pe

    void handlePreparationPhaseEvents(const SDL_Event& e) const;
    void handleCombatPhaseEvents(const SDL_Event& e) const;

    // para deteccion de teclas de forma discreta (sin mantener apretado)
    void handleKeyDown(SDL_Scancode sc) const;
    // para deteccion de boton apretado de forma discreta (sin mantener apretado)
    void handleMouseButtonDown(const SDL_MouseButtonEvent& b) const;

    void handleKeyboardEvents(bool& gameIsRunning);
    void handleMouseEvents(bool gameIsRunning);

    // TODO: Esta funcion deberia ser utilitaria (no deberia estar en EventHandler).
    bool isAutomatic(Weapon weapon) const;

public:
    EventHandler(Client& client, World& world, Shop& shop);

    void handleEvents(bool& gameIsRunning, GamePhase gamePhase);
};

#endif  // EVENTHANDLER_H
