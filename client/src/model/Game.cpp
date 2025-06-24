#include "../../../client/include/model/Game.h"

#include <chrono>
#include <thread>

#include "client/client_constants.h"
#include "client/include/model/StatisticsPanel.h"
#include "server/include/types2.h"

Game::Game(Client& client, const MatchInfo& match_info):
        client(client),
        graphics(match_info.win_config, match_info.fovConfig, match_info.name),
        world(graphics, audio, match_info.tileMap, match_info.win_config, match_info.numPlayers,
              match_info.localPlayerInfo),
        shop(graphics, match_info.shopInfo, SHOP_FONT_FILE_NAME, SHOP_FONT_SIZE),
        eventHandler(client, world, shop),
        is_running(true) {}

void Game::update(const float dt) {
    // Es importante que la comunicacion con el server sea asincronica.
    // El cliente debe poder renderizar sin todavia haber recibido informacion del servidor.
    // De esta forma independizamos los FPS del gameloop del cliente del tickrate del servidor.
    auto maybeNewGameInfo = client.tryGetGameInfo();
    if (maybeNewGameInfo.has_value()) {
        lastGameInfo = maybeNewGameInfo.value();
        world.update(lastGameInfo, dt);
    }

    switch (lastGameInfo.gamePhase) {
        case GamePhase::Combat:
            if (audio.isMusicPlaying())
                audio.haltMusic();
            break;
        case GamePhase::Preparation:
        case GamePhase::EndOfMatch:
            audio.haltAllChannels();
            if (!audio.isMusicPlaying())
                audio.playLoopMusic(0.5);
            break;
    }
    graphics.updateMouse();
}

void Game::render() {
    graphics.clear();
    world.render();

    switch (lastGameInfo.gamePhase) {
        case GamePhase::Preparation: {
            shop.render();
            break;
        }
        case GamePhase::EndOfMatch: {
            StatisticsPanel stats(graphics, lastGameInfo.stats, SHOP_FONT_FILE_NAME);
            stats.render();
            break;
        }
        case GamePhase::Combat:
            break;
        default:
            break;
    }
    graphics.renderMouse();  // El mouse debe ser lo ultimo que renderizamos.
    graphics.present();
}

void Game::start() {
    using clock = std::chrono::steady_clock;
    using seconds = std::chrono::duration<float>;

    constexpr float target_fps = 60.0f;
    constexpr seconds target_frame_duration(1.0f / target_fps);
    constexpr seconds max_accumulated_time(
            0.25f);  // Saltea como máximo 0.25s de lógica para no congelarse

    auto previous = clock::now();
    // int frame_count = 0;
    // auto fps_timer = clock::now();

    while (is_running) {
        const auto current = clock::now();
        seconds frame_time = current - previous;

        // Si por algún motivo el frame_time se descontrola, capémoslo
        if (frame_time > max_accumulated_time)
            frame_time = max_accumulated_time;

        previous = current;

        // Lógica de juego (usa delta time)
        eventHandler.handleEvents(is_running, lastGameInfo.gamePhase);
        update(frame_time.count());  // en segundos. O quizas para actualizar las animaciones
                                     // deberia utilizar el clock del servidor?
        render();

        // Esperar el tiempo restante del frame
        const auto post_logic = clock::now();
        seconds elapsed = post_logic - current;

        if (elapsed < target_frame_duration) {
            std::this_thread::sleep_for(target_frame_duration - elapsed);
        }

        // FPS debug cada 1 segundo
        // ++frame_count;
        // if ((clock::now() - fps_timer) >= seconds(1.0f)) {
        //     std::cout << "FPS: " << frame_count << std::endl;
        //     frame_count = 0;
        //     fps_timer = clock::now();
        // }
    }
}

Game::~Game() {}
