#include "client/include/model/Game.h"

Game::Game(const MatchInfo& match_info, Client& client):
        client(client),
        match_name(match_info.name),
        graphics(match_info.win_config, match_name),
        world(match_info.first_snap, match_info.map_info, match_info.win_config),
        eventHandler(client),
        is_running(true) {}

void Game::update(float dt) { world.update(dt, client.getGameSnapshot()); }

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
