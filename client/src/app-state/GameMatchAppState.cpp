#include "../../../client/include/app-state/GameMatchAppState.h"

#include <string>

#include <SDL2pp/SDL2pp.hh>

#include "../../../client/include/app-state/AppStateController.h"
#include "../../../client/include/model/Game.h"

GameMatchAppState::GameMatchAppState(AppStateController* ctrl) { controller = ctrl; }

std::optional<AppStateCode> GameMatchAppState::update() {
    try {
        const auto client = controller->getClient();
        const auto matchInfo = client->getMatchInfo();
        matchInfo.print(); // TODO: eliminar

        Game game(*client, matchInfo);

        game.start();

        return AppStateCode::QUIT; // TODO: enviar a menu luego de terminar la partida.
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
