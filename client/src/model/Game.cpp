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
    // graphics.updateMouse();
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
    // graphics.renderMouse(); // El mouse debe ser lo ultimo que renderizamos.
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
    int frame_count = 0;
    auto fps_timer = clock::now();

    while (is_running) {
        const auto current = clock::now();
        seconds frame_time = current - previous;

        // Si por algún motivo el frame_time se descontrola, capémoslo
        if (frame_time > max_accumulated_time)
            frame_time = max_accumulated_time;

        previous = current;

        // Lógica de juego (usa delta time)
        eventHandler.handleEvents(is_running, lastGameInfo.gamePhase);
        // auto start = std::chrono::steady_clock::now();
        update(frame_time.count());  // en segundos. O quizas para actualizar las animaciones
                                     // deberia utilizar el clock del servidor?
        // auto afterUpdate = std::chrono::steady_clock::now();
        render();
        // auto afterRender = std::chrono::steady_clock::now();

        // Esperar el tiempo restante del frame
        const auto post_logic = clock::now();
        seconds elapsed = post_logic - current;

        if (elapsed < target_frame_duration) {
            std::this_thread::sleep_for(target_frame_duration - elapsed);
        }

        // FPS debug cada 1 segundo
        ++frame_count;
        if ((clock::now() - fps_timer) >= seconds(1.0f)) {
            // std::cout << "FPS: " << frame_count << std::endl;
            frame_count = 0;
            fps_timer = clock::now();
        }
    }
}

Game::~Game() {}


// void Game::start() {
//     using clock = std::chrono::high_resolution_clock;
//     using time_point = clock::time_point;
//
//     const std::chrono::duration<float> target_frame_duration(1.0f / FPS);
//
//     time_point previous = clock::now();
//     const int MAX_UPDATES_PER_FRAME = 3;
//     float accumulator = 0.0f;
//     const float dt = 1.0f / FPS;
//
//     while (is_running) {
//         auto start = clock::now();
//
//         eventHandler.handleEvents(is_running);
//
//         accumulator += (start - previous).count();
//         previous = start;
//
//         int updates = 0;
//         while (accumulator >= dt && updates < MAX_UPDATES_PER_FRAME) {
//             update(dt);
//             accumulator -= dt;
//             updates++;
//         }
//
//         render();  // Idealmente con interpolación usando accumulator/dt
//
//         auto end = clock::now();
//         auto frame_time = end - start;
//
//         if (frame_time < target_frame_duration) {
//             std::this_thread::sleep_for(target_frame_duration - frame_time);
//         }
//     }
// }


// void Game::start() {
//     using clock = std::chrono::high_resolution_clock;
//     using time_point = clock::time_point;
//
//     const std::chrono::duration<float> target_frame_duration(1.0f / FPS);
//
//     time_point previous = clock::now();
//
//     while (is_running) {
//         time_point start = clock::now();
//
//         eventHandler.handleEvents(is_running);
//
//         // Aquí asumimos que el snapshot ya fue recibido por el cliente.
//         const auto now = clock::now();
//         const float dt = std::chrono::duration<float>(now - previous).count();
//         previous = now;
//
//         update(dt);   // Recibe el snapshot y actualiza mundo local
//         render();     // Renderiza mundo interpolado si querés
//
//         // Sleep si terminó antes del target
//         const auto end = clock::now();
//         const auto frame_time = end - start;
//
//         if (frame_time < target_frame_duration) {
//             std::this_thread::sleep_for(target_frame_duration - frame_time);
//         }
//     }
// }


// void Game::start() {
//     constexpr float FIXED_DT = 1.0f / 60.0f;  // 60 updates por segundo
//     constexpr float MAX_ACCUMULATED_TIME = 0.25f;
//
//     float accumulator = 0.0f;
//     auto previous = std::chrono::steady_clock::now();
//
//     while (is_running) {
//         auto now = std::chrono::steady_clock::now();
//         float frame_time = std::chrono::duration<float>(now - previous).count();
//         previous = now;
//
//         frame_time = std::min(frame_time, MAX_ACCUMULATED_TIME);
//         accumulator += frame_time;
//
//         eventHandler.handleEvents(is_running);
//
//         while (accumulator >= FIXED_DT) {
//             update(FIXED_DT);
//             accumulator -= FIXED_DT;
//         }
//
//         render();
//     }
// }
