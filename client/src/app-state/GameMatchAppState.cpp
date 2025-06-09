#include "../../../client/include/app-state/GameMatchAppState.h"

#include <random>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../../../client/client_constants.h"
#include "../../../client/include/app-state/AppStateController.h"
#include "../../../client/include/client.h"
#include "../../../client/include/model/Game.h"
#include "../../../common/dtos/MatchInfo.h"

GameMatchAppState::GameMatchAppState(AppStateController* ctrl) { controller = ctrl; }

// Todas las configuraciones iniciales de la partida las tiene que recibir el constructor
// de GameMatchAppState, que provienen del MainMenuState, cuando el usuario
// selecciona la partida a la que se quiere unir o cuando crea una.
// Esta info que recibe corresponde a la info estatica de la que el profe nos hablaba.
// GameMatchAppState::GameMatchAppState(const MatchInfo& match_info) {}

std::optional<AppStateCode> GameMatchAppState::update() {
    try {
        constexpr int SERVER_ENTITY_ID = 1;
        constexpr float pos_x = 30, pos_y = 30, angle = 0, money = 500;
        constexpr int hp = 100;  // health
        constexpr auto sprite_type = SpriteType::SEAL_FORCE;
        constexpr auto entt_type = EntityType::ANTI_TERRORIST;
        constexpr bool is_alive = true;
        const EntitySnapshot first_snap(SERVER_ENTITY_ID, pos_x, pos_y, angle, sprite_type,
                                        entt_type, hp, money, Team::Terrorist, PlayerState::Idle,
                                        is_alive);

        // TODO. Sacar el first snap del constructor y dejar que lo cree al recibirlo por el
        // snapshot del juego.
        MatchInfo matchInfo = controller->getClient()->getMatchInfo();
        matchInfo.print();

        //         for (int y = 0; y < h; ++y) {
        //             for (int x = 0; x < w; ++x) {
        //                 tileMap[y][x] = dist(gen);
        //             }
        //         }
        // const MapInfo map_info(tileMap, SpriteType::DESERT_MAP, w, h);

        // const MatchInfo match_info("Partidita", first_snap, win_config, map_info);

        // Client client("localhost", "8080", "seba");
        const auto client = controller->getClient();

        Game game(first_snap, matchInfo, client);

        game.start();

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
