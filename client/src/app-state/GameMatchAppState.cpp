#include "client/include/app-state/GameMatchAppState.h"

#include <string>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "client/dtos/matchInfo.h"
#include "client/include/model/Game.h"

GameMatchAppState::GameMatchAppState() {}

// Todas las configuraciones iniciales de la partida las tiene que recibir el constructor
// de GameMatchAppState, que provienen del MainMenuState, cuando el usuario
// selecciona la partida a la que se quiere unir o cuando crea una.
// Esta info que recibe corresponde a la info estatica de la que el profe nos hablaba.
// GameMatchAppState::GameMatchAppState(const match_info_t match_info) {}
// match_info_t {
//     const std::string name;
//     const EntitySnapshot first_snap;  // Representa el snapshot inicial del local_player
//     server. const window_config_t win_config; const std::string map_scene; // filename del mapa a
//     utilizar? const std::list<players_info_t> players;  // pensar si es la mejor forma.
// };
// La informacion de si la ventana se abre en fullscreen o no viene dada en el archivo
// de configuracion con el que se inicia el server. Lo mismo para la resolucion.
// Por lo tanto, por mas que suene antuitivo, esa info la tenemos que recibir del server.
// Personalmente me gustaria que eso lo decida el propio usuario que va a jugar.
// (Preguntar esto ultimo).
std::optional<AppStateCode> GameMatchAppState::update() {
    try {
        constexpr int SERVER_ENTITY_ID = 1;
        float pos_x = 20, pos_y = 50, angle = 0, money = 500;
        int hp = 100;  // health
        const SpriteType sprite_type = SpriteType::SEAL_FORCE;
        const EntityType entt_type = EntityType::ANTI_TERRORIST;
        const bool is_alive = true;
        const EntitySnapshot first_snap(SERVER_ENTITY_ID, pos_x, pos_y, angle, sprite_type,
                                        entt_type, hp, money, is_alive);
        const window_config_t win_config(
                640, 400, SDL_WINDOW_SHOWN);  // SDL_WINDOW_FULLSCREEN | SDL_WINDOW_SHOWN
        const std::string map_image = "client/assets/backgrounds/temp_map.png";
        const match_info_t match_info("Partidita", first_snap, win_config, map_image);
        Game game(match_info);

        game.game_loop();

        return AppStateCode::MAIN_MENU;
    } catch (const SDL2pp::Exception& e) {
        std::cerr << "Fatal error: " << e.what() << ", SDL error: " << e.GetSDLError() << std::endl;
        // Deberia lanzar otro tipo de excepcion para manejar en el AppStateController?
        // Quizas puedo retornar al cliente al lobby si ocurre un problema al iniciar la
        // partida o durante la misma.
        // Otra opcion es mostrar una ventana con el error y preguntar si desea cerrar el programa
        // o volver al lobby.
        return AppStateCode::MAIN_MENU;
    }
}

GameMatchAppState::~GameMatchAppState() {}
