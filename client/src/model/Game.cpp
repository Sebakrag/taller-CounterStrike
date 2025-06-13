#include "client/include/model/Game.h"

#include "common/dtos/MapInfo.h"
#include "client/include/model/utils/SpriteType.h"

Game::Game(const MatchInfo& match_info, Client& client):
        client(client),
        match_name(match_info.name),
        graphics(match_info.win_config, match_name),
        world(
            // Si hay datos de juego, usar el primer snapshot para el jugador local
            // Si no hay, usar un EntitySnapshot vacío como fallback
            !client.getGameInfo().empty() ? client.getGameInfo().front() : EntitySnapshot(),
            // Convertir TileMap a formato MapInfo
            [&match_info]() {
                // Crear una matriz de enteros basada en TileMap
                std::vector<std::vector<int>> tileData;
                const int rows = match_info.tileMap.getRowCount();
                const int cols = match_info.tileMap.getColCount();
                
                // Extraer los ID de los tiles
                for (int i = 0; i < rows; i++) {
                    std::vector<int> row;
                    for (int j = 0; j < cols; j++) {
                        row.push_back(match_info.tileMap.getIdTile(i, j));
                    }
                    tileData.push_back(row);
                }
                
                // Determinar el tipo de sprite basado en el tipo de TileMap
                SpriteType tileSetType;
                switch (match_info.tileMap.getType()) {
                    case TypeTileMap::Forest:
                        tileSetType = SpriteType::FOREST_TILE_SET;
                        break;
                    case TypeTileMap::Snow:
                        tileSetType = SpriteType::SNOW_TILE_SET;
                        break;
                    case TypeTileMap::Desert:
                    default:
                        tileSetType = SpriteType::DESERT_TILE_SET;
                        break;
                }
                
                return MapInfo(tileData, tileSetType, cols, rows);
            }(),
            match_info.win_config
        ),
        eventHandler(client, world),
        is_running(true) {}

void Game::update(float dt) {
    // Cuando refactoricemos getGameInfo para que devuelva un GameInfo tenemos que modificar
    // esta funcion para decidir en que fase del juego nos encontramos.
    world.update(dt, client.getGameInfo());
}

void Game::render() { graphics.render(world); }

void Game::start() {
    float dt = 0;
    while (is_running) {
        eventHandler.handleEvents(is_running);
        update(dt);
        render();

        usleep(1 / 30);  // adjust frame_rate
    }
}

Game::~Game() {}
