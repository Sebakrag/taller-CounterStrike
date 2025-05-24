#include "client/include/model/Game.h"

#include <SDL2/SDL.h>

Game::Game(const match_info_t& match_info):
        match_name(match_info.name),
        graphics(match_info.win_config, match_name),
        // aca hay que inicializar todos los objetos del juego: mapa, jugadores... algo mas?
        // map(renderer, match_info.map_scene, window),
        world(match_info.first_snap),
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
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        is_running = false;
                        break;

                    default:
                        break;
                }
            }  // end SDL_KEYDOWN
            default:
                break;
        }  // end switch(e.type)

        // handle_restrictive_event(e);  // this function analyze if the event is one that the
        // server
        //  needs to check. So then, the action is send to the server.

        world.forward_event(e);  // Here we forward an event that it's something we can do without
                                 // the permission of the server (for ex.: rotate the vision).
    }                            // end while(SDL_PollEvent)
}

// void Game::handle_server_messages() {
//     auto snapshots = client.get_game_snapshot();
//     world.handle_game_snapshot(snapshots);
// }

void Game::update(float dt) { world.update(dt); }

void Game::render() { graphics.render(world); }

void Game::game_loop() {
    float dt = 0;
    while (is_running) {
        handle_events();
        // handle_server_messages(); // receive info from the server and parse it into components.
        update(dt);  // I could have a SendUpdateToServerSystem that is responsible for sending the
                     // update of our local_player to the server.
                     // More than "update", I should send the event that the player produce.
        render();

        usleep(1 / 30);  // adjust frame_rate
    }
}

Game::~Game() {}
