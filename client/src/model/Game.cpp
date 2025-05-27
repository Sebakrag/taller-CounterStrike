#include "client/include/model/Game.h"

Game::Game(const match_info_t& match_info, Client& client):
        client(client),
        match_name(match_info.name),
        graphics(match_info.win_config, match_name),
        world(match_info.first_snap, match_info.map_info),
        eventHandler(client),
        is_running(true) {}

void Game::handleServerMessages() {
    const auto snapshots = client.getGameSnapshot();
    world.handleGameSnapshot(snapshots);
}

void Game::update(float dt) { world.update(dt); }

void Game::render() { graphics.render(world); }

void Game::start() {
    float dt = 0;
    while (is_running) {
        eventHandler.handleEvents(is_running);
        handleServerMessages();  // receive info from the server and parse it into components.
        update(dt);  // I could have a SendUpdateToServerSystem that is responsible for sending the
                     // update of our local_player to the server.
                     // More than "update", I should send the event that the player produce.
        render();

        usleep(1 / 30);  // adjust frame_rate
    }
}

Game::~Game() {}
