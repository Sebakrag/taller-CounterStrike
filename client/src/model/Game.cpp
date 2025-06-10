#include "client/include/model/Game.h"

Game::Game(const EntitySnapshot& firstLocalPlayerSnap, const MatchInfo& match_info, Client* client):
        client(client),
        match_name(match_info.name),
        graphics(match_info.win_config, match_name),
        world(firstLocalPlayerSnap, match_info.tileMap, match_info.win_config),
        eventHandler(client, world),
        is_running(true) {}

void Game::update(float dt) {
    // Cuando refactoricemos getGameInfo para que devuelva un GameInfo tenemos que modificar
    // esta funcion para decidir en que fase del juego nos encontramos.
    world.update(dt, client->getGameInfo());
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
