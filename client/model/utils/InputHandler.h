#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <SDL2/SDL.h>

#include "client/model/ECS/ComponentManager.h"
#include "client/model/ECS/Entity.h"

class InputHandler {
private:
    const Entity local_player;

public:
    explicit InputHandler(const Entity local_player);

    // Do I need to pass a float delta_time?
    void handle_event(const SDL_Event& e, ComponentManager& cm);
};

#endif  // INPUTHANDLER_H
