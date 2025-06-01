#include "../include/gameloop.h"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <utility>

GameLoop::GameLoop(Match&& match, std::list<std::shared_ptr<Queue<GameInfo>>> queuesPlayers):
        match(std::move(match)),
        queueActionsPlayers(std::make_shared<Queue<PlayerAction>>()),
        queuesPlayers(queuesPlayers) {}

void GameLoop::run() {
    std::cout << "Gameloop Start." << std::endl;
    std::chrono::milliseconds time_expected = std::chrono::milliseconds(1000 / ITR_PER_SEC);
    int it = 0;  //(ver si esto lo necesito)
    int seconds = 0;
    try {
        while (should_keep_running()) {
            auto time_start = std::chrono::steady_clock::now();

            // 1. recibo todas las acciones de los jugadores y las proceso
            PlayerAction playerAction;
            while (queueActionsPlayers->try_pop(playerAction)) {
                // habría que poner una condicion de temporizador o cantidad de iteraciones maxima
                match.processAction(playerAction, 1.0f/ ITR_PER_SEC);
            }

            match.updateState(1.0 / ITR_PER_SEC);

            // 2. envío el estado de juego a cada jugador
            for (auto queue: queuesPlayers) {
                queue->try_push(match.generateGameInfo());
            }

            // 3. calculo cuanto tiempo dormir.
            auto time_end = std::chrono::steady_clock::now();
            auto itr_time = time_end - time_start;
            if (itr_time < time_expected) {
                std::this_thread::sleep_for(time_expected - itr_time);  // sleep
            }
            // si tardé mas que el time_expected, ni duermo.
            it++;

            // calculo cuando pasó un segundo.
            if (it % ITR_PER_SEC == 0) {
                std::cout << std::to_string(seconds) << std::endl;
                seconds++;
                it = 0;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "GameLoop: " << e.what() << std::endl;
    }
}

void GameLoop::kill() {
    queueActionsPlayers->close();
    for (auto queue: queuesPlayers) {
        queue->close();
    }
    stop();
}
std::shared_ptr<Queue<PlayerAction>> GameLoop::getActionsQueue() { return queueActionsPlayers; }
