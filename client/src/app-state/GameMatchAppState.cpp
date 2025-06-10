#include "client/include/app-state/GameMatchAppState.h"

#include <string>

#include <SDL2pp/SDL2pp.hh>

#include "client/include/app-state/AppStateController.h"
#include "client/include/client.h"
#include "client/include/model/Game.h"
#include "common/dtos/MatchInfo.h"

GameMatchAppState::GameMatchAppState(AppStateController* ctrl) { controller = ctrl; }

// Todas las configuraciones iniciales de la partida las tiene que recibir el constructor
// de GameMatchAppState, que provienen del MainMenuState, cuando el usuario
// selecciona la partida a la que se quiere unir o cuando crea una.
// Esta info que recibe corresponde a la info estatica de la que el profe nos hablaba.
// GameMatchAppState::GameMatchAppState(const MatchInfo& match_info) {}

std::optional<AppStateCode> GameMatchAppState::update() {
    try {
        constexpr ServerEntityID SERVER_ENTITY_ID = 1;
        constexpr float pos_x = 30500, pos_y = 30500, angle = 0;
        constexpr int money = 1000;
        constexpr int hp = 200;  // health
        constexpr int ammo = 99;
        constexpr auto sprite_type = SpriteType::ARTIC_AVENGER;
        constexpr auto entt_type = EntityType::PLAYER;
        constexpr bool is_alive = true;
        constexpr ServerEntityID equipped_weapon_id = 6;  // Arbitrario.
        const EntitySnapshot first_snap(SERVER_ENTITY_ID, entt_type, sprite_type, pos_x, pos_y,
                                        angle, is_alive, hp, money, ammo, PlayerState::Idle,
                                        equipped_weapon_id, Team::Terrorist);

        // TODO. Sacar el first snap del constructor y dejar que lo cree al recibirlo por el
        // snapshot del juego.
        const MatchInfo matchInfo = controller->getClient()->getMatchInfo();
        matchInfo.print();

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
