#include "GameMatchAppState.h"

#include <string>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include <SDL_image.h>

using namespace SDL2pp;

GameMatchAppState::GameMatchAppState() {}

std::optional<AppStateCode> GameMatchAppState::update() {
    try {
        // Aca estaria el codigo de la grafica del juego en si y el game loop propiamente dicho.
        SDL sdl(SDL_INIT_VIDEO);
        SDLImage sdl_image(IMG_INIT_PNG | IMG_INIT_JPG);
        SDLTTF sdl_ttf;

        // Cuando recibamos info del server podemos cambiar el nombre de la ventana por:
        // "Counter Strike - <nombre_partida>".
        // La informacion de si la ventana se abre en fullscreen o no viene dada en el archivo
        // de configuracion con el que se inicia el server. Lo mismo para la resolucion.
        // Por lo tanto, por mas que suene antuitivo, esa info la tenemos que recibir del server.
        // Personalmente me gustaria que eso lo decida el propio usuario que va a jugar.
        // (Preguntar esto ultimo).
        Window window("Counter Strike", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 400,
                      SDL_WINDOW_SHOWN);  // SDL_WINDOW_FULLSCREEN | SDL_WINDOW_SHOWN

        Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);
        // La imagen del escenario que vamos a utilizar la debo recibir del server (pongo una
        // provisoria).
        const std::string map_image = "client/assets/backgrounds/temp_map.png";
        // const std::string map_image = "client/assets/backgrounds/menu_background.jpg";
        Texture match_background(renderer, map_image);

        // GameWorld world(match_background);
        // bool running = true;
        // while (running) {
        //     running = handle_events();
        //     //update();
        //     // render(renderer, world);
        // }

        for (int i = 0; i < 500; ++i) {
            renderer.Clear();
            renderer.Copy(match_background, Rect(0 + i, 0 + i, 640, 400));
            renderer.Present();
            usleep(1 / 30);
        }

        SDL_Delay(5000);

        return AppStateCode::MAIN_MENU;
    } catch (const Exception& e) {
        std::cerr << "Fatal error: " << e.what() << ", SDL error: " << e.GetSDLError() << std::endl;
        // Deberia lanzar otro tipo de excepcion para manejar en el AppStateController?
        // Quizas puedo retornar al cliente al lobby si ocurre un problema al iniciar la
        // partida o durante la misma.
        // Otra opcion es mostrar una ventana con el error y preguntar si desea cerrar el programa
        // o volver al lobby.
        return AppStateCode::MAIN_MENU;
    }
}

bool GameMatchAppState::handle_events() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT: {
                return false;
            }
            case SDL_KEYDOWN: {
                auto& keyEvent = reinterpret_cast<SDL_KeyboardEvent&>(e);
                switch (keyEvent.keysym.sym) {
                    case SDLK_LEFT:

                        break;
                    case SDLK_RIGHT:

                        break;
                }
            }  // end SDL_KEYDOWN
        }      // end switch(e.type)

    }  // end while(SDL_PollEvent)

    return true;
}


GameMatchAppState::~GameMatchAppState() {}
