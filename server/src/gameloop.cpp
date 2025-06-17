#include "../include/gameloop.h"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <utility>

GameLoop::GameLoop(Match& match,
                   const std::map<std::string, std::shared_ptr<Queue<GameInfo>>>& queuesPlayers):
        match(match),
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
                match.processAction(playerAction, 1.0f / ITR_PER_SEC);
            }

            match.updateState(1.0 / ITR_PER_SEC);
            if (match.getGamePhase() == GamePhase::EndOfMatch) {
                std::cout << "==> Fase de fin de partida alcanzada. Finalizando el Gameloop\n";
                //stop(); CONSULTAR ESTO, DEBERIA CORTAR?
            }

            // 2. envío el estado de juego a cada jugador
            for (auto it = queuesPlayers.begin(); it != queuesPlayers.end();) {
                std::string user_name = it->first;
                auto queue = it->second;
                try {
                    queue->try_push(match.generateGameInfo(user_name));
                    ++it;
                } catch (ClosedQueue& e) {  // si un jugador se fue de la partida (cerró el juego)
                    it = queuesPlayers.erase(it);  // borro queue y continúo
                    std::cout << "borro queue" << std::endl;
                    if (queuesPlayers.size() == 0) {
                        // si se fueron todos los jugadores, termina el gameloop
                        stop();
                    }
                }
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
    std::cout << "Gameloop fuera." << std::endl;
}

void GameLoop::kill() {
    queueActionsPlayers->close();
    for (auto queue: queuesPlayers) {
        // queue->close();
    }
    stop();
}

// bool GameLoop::containsPlayer(const std::string& username) const {
//     return match.containsPlayer(username);
// }

// PlayerInfo GameLoop::getPlayerInfo(const std::string& username) const {
//     return match.generatePlayerInfo(username);
// }
const Match& GameLoop::getMatch() const { return match; }
std::shared_ptr<Queue<PlayerAction>> GameLoop::getActionsQueue() { return queueActionsPlayers; }
