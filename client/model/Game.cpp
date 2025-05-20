#include "Game.h"

#include <SDL2/SDL.h>

Game::Game(const match_info_t& match_info):
        match_name(match_info.get_name()),
        graphics(match_info.get_window_config(), match_name),
        // aca hay que inicializar todos los objetos del juego: mapa, jugadores... algo mas?
        // map(renderer, match_info.get_map_scene(), window),
        is_running(true) {}

void Game::handle_events() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT: {
                is_running = false;
                break;
            }
            case SDL_KEYDOWN: {
                auto& keyEvent = reinterpret_cast<SDL_KeyboardEvent&>(e);
                switch (keyEvent.keysym.sym) {
                    case SDLK_ESCAPE:
                        is_running = false;
                        break;
                }
            }  // end SDL_KEYDOWN
        }      // end switch(e.type)

        world.forward_event(e);
    }  // end while(SDL_PollEvent)
}

void Game::update(float dt) { world.update(dt); }

void Game::render() { graphics.render(world); }

void Game::game_loop() {
    float dt = 0;
    while (is_running) {
        // handle_server_messages(); // receive info from the server and handle it.
        handle_events();
        update(dt);  // I could have a SendUpdateToServerSystem that is responsible for sending the
                     // update of our local_player to the server.
        render();

        usleep(1 / 30);  // adjust frame_rate
    }
}

Game::~Game() {}
