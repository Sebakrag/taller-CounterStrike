#include "../../../client/include/model/Game.h"

Game::Game(Client& client, const MatchInfo& match_info):
        client(client),
        graphics(match_info.win_config, match_info.name),
        world(match_info.tileMap, match_info.win_config, match_info.numPlayers,
              match_info.localPlayerInfo),
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

        usleep(1 / 24);  // adjust frame_rate
    }
}

Game::~Game() {}
