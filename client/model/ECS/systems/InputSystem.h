#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <SDL2/SDL.h>

#include "client/model/ECS/System.h"

// It could be re-name to "InputHandler"
class InputSystem {
private:
    const Entity local_player;

public:
    explicit InputSystem(const Entity local_player);

    // Do I need to pass a float delta_time?
    void handle_event(const SDL_Event& e, ComponentManager& cm);
};

#endif  // INPUTSYSTEM_H
