#include "client/include/app-state/GameMatchAppState.h"

#include <random>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "client/client_constants.h"
#include "client/include/app-state/AppStateController.h"
#include "client/include/client.h"
#include "client/include/model/Game.h"
#include "common/dtos/MatchInfo.h"
#include "client/include/model/EC/EntityType.h"
#include "common/tile_map.h" // Para TileMap
#include "common/types.h" // Para TypeTileMap

GameMatchAppState::GameMatchAppState(AppStateController* ctrl) { controller = ctrl; }

// Todas las configuraciones iniciales de la partida las tiene que recibir el constructor
// de GameMatchAppState, que provienen del MainMenuState, cuando el usuario
// selecciona la partida a la que se quiere unir o cuando crea una.
// Esta info que recibe corresponde a la info estatica de la que el profe nos hablaba.
// GameMatchAppState::GameMatchAppState(const MatchInfo& match_info) {}

std::optional<AppStateCode> GameMatchAppState::update() {
    try {
        // Configuración de la ventana
        const WindowConfig win_config(
                SCREEN_WIDTH, SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN);  // SDL_WINDOW_FULLSCREEN | SDL_WINDOW_SHOWN

        // Crear un mapa de tiles aleatorio
        constexpr int w = 1000;
        constexpr int h = 1000;
        std::vector<std::vector<int>> tileMapData(h, std::vector<int>(w));
        std::random_device rd;                        // fuente de entropía
        std::mt19937 gen(rd());                       // motor de generación
        std::uniform_int_distribution<> dist(1, 46);  // distribución uniforme

        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                tileMapData[y][x] = dist(gen);
            }
        }
        const MapInfo map_info(tileMapData, SpriteType::DESERT_MAP, w, h);
        
        // Crear un objeto TileMap a partir de los datos del mapa
        // Usamos TypeTileMap::Desert como ejemplo, ajustar según sea necesario
        TileMap tileMap(TypeTileMap::Desert, tileMapData);

        // Crear MatchInfo con los parámetros correctos: nombre, configuración de ventana, mapa de tiles, número de jugadores
        const MatchInfo match_info("Partidita", win_config, tileMap, 1);

        // Client client("localhost", "8080", "seba");
        const auto client = controller->getClient();

        Game game(match_info, *client);

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
