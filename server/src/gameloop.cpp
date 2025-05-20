
#include "../include/gameloop.h"

#include <chrono>
#include <thread>
#include <utility>

GameLoop::GameLoop(Match&& match, std::shared_ptr<Queue<PlayerAction>> queueActionsPlayers,
                   std::list<std::shared_ptr<Queue<GameInfo>>> queuesPlayers):
        match(std::move(match)),
        queueActionsPlayers(queueActionsPlayers),
        queuesPlayers(queuesPlayers) {}

void GameLoop::run() {
    std::chrono::milliseconds time_expected = std::chrono::milliseconds(1000 / ITR_PER_SEC);
    int it = 0;  //(ver si esto lo necesito)
    try {
        while (should_keep_running()) {
            auto time_start = std::chrono::steady_clock::now();

            // 1. recibo todas las acciones de los jugadores y las proceso
            PlayerAction playerAction;
            while (queueActionsPlayers->try_pop(playerAction)) {
                // habría que poner una condicion de temporizador o cantidad de iteraciones maxima
                match.processAction(playerAction);
            }

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
        }
    } catch (const std::exception& e) {
        std::cerr << "GameLoop error: " << e.what() << std::endl;
    }
}
