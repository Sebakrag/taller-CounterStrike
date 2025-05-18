#ifndef MATCH_H_
#define MATCH_H_

#include <string>
#include <vector>

#include "../common/thread.h"

class GameManager;  // forward declaration


// solo el player host podrá iniciar el hilo (llamar a start())

class Match: public Thread {
    friend class GameManager;  // 🔒 Solo GameManager puede ver lo privado de Match
private:
    std::string playerHost;
    std::vector<std::string> players;  // por ahora
public:
    Match(const std::string& username);

    std::vector<std::string> getPlayers();
    ~Match();

private:
    void run() override;
    void start() override;

    bool addPlayer(const std::string& username);

    void removePlayer(const std::string& username);

    /**
     * devuelve true si la partida aún no ha comenzado y hay espacio
     * libre para que se unan mas jugadores.
     */
    bool isAvailable();
};

#endif  // MATCH_H_
