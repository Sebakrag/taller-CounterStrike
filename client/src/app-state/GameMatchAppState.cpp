#include "../../../client/include/app-state/GameMatchAppState.h"

#include <string>

#include <SDL2pp/SDL2pp.hh>

#include "../../../client/include/app-state/AppStateController.h"
#include "../../../client/include/model/Game.h"

GameMatchAppState::GameMatchAppState(AppStateController* ctrl) { controller = ctrl; }

// Todas las configuraciones iniciales de la partida las tiene que recibir el constructor
// de GameMatchAppState, que provienen del MainMenuState, cuando el usuario
// selecciona la partida a la que se quiere unir o cuando crea una.
// Esta info que recibe corresponde a la info estatica de la que el profe nos hablaba.
// GameMatchAppState::GameMatchAppState(const MatchInfo& match_info) {}

std::optional<AppStateCode> GameMatchAppState::update() {
    try {
        const MatchInfo matchInfo = controller->getClient()->getMatchInfo();
        matchInfo.print();

        const auto client = controller->getClient();

        Game game(*client, matchInfo);

        game.start();

        return AppStateCode::QUIT;
    } catch (const SDL2pp::Exception& e) {
        std::cerr << "Fatal error: " << e.what() << ", SDL error: " << e.GetSDLError() << std::endl;
        // Deberia lanzar otro tipo de excepcion para manejar en el AppStateController?
        // Quizas puedo retornar al cliente al lobby si ocurre un problema al iniciar la
        // partida o durante la misma.
        // Otra opcion es mostrar una ventana con el error y preguntar si desea cerrar el programa
        // o volver al lobby.
        return AppStateCode::QUIT;
    }
}

GameMatchAppState::~GameMatchAppState() {}
