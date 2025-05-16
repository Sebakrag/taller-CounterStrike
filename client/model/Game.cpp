#include "Game.h"

#include <SDL2/SDL.h>

Game::Game(const match_info_t& match_info, const uint32_t img_flags):
        match_name(match_info.get_name()),
        sdl(SDL_INIT_VIDEO),
        sdl_image(img_flags),
        sdl_ttf(),
        window(create_window(match_info)),
        renderer(create_renderer(window)),
        // aca hay que inicializar todos los objetos del juego: mapa, jugadores... algo mas?
        map(renderer, match_info.get_map_scene(), window),
        is_running(true) {}

Window Game::create_window(const match_info_t& match_info) const {
    const std::string win_title = std::string(GAME_NAME) + " - " + match_name;
    const window_config_t config = match_info.get_window_config();
    return Window(win_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, config.get_widht(),
                  config.get_height(), config.get_flags());
}

Renderer Game::create_renderer(Window& window) {
    Renderer ren(window, -1, SDL_RENDERER_ACCELERATED);
    ren.SetDrawColor(255, 255, 255, 255);
    return ren;
}

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
                    case SDLK_LEFT:

                        break;
                    case SDLK_RIGHT:

                        break;
                }
            }  // end SDL_KEYDOWN
        }      // end switch(e.type)
    }          // end while(SDL_PollEvent)
}

void Game::update() { map.update(); }

void Game::render() {
    renderer.Clear();
    // Aca creamos la imagen renderizada aniadiendo capas.
    map.render();
    renderer.Present();
}


bool Game::running() const { return is_running; }

Game::~Game() {}
