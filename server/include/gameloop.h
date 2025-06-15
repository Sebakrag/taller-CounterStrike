#ifndef GAMELOOP_H_
#define GAMELOOP_H_

#include <list>
#include <map>
#include <memory>
#include <string>

#include "../../common/game_info/game_info.h"
#include "../../common/queue.h"
#include "../../common/thread.h"
#include "../../common/types.h"

#include "match.h"

#define ITR_PER_SEC 24

// class GameManager;  // forward declaration

class GameLoop: public Thread {
    //    friend class GameManager;  // 🔒 Solo GameManager puede ver lo privado de Match
private:
    Match match;
    std::shared_ptr<Queue<PlayerAction>> queueActionsPlayers;  // recurso compartido

    // queues senders. cada queue es un recurso compartido con los senders
    // std::list<std::shared_ptr<Queue<GameInfo>>> queuesPlayers;
    std::map<std::string, std::shared_ptr<Queue<GameInfo>>> queuesPlayers;

public:
    GameLoop(Match&& match,
             const std::map<std::string, std::shared_ptr<Queue<GameInfo>>>& queuesPlayers);

    /**
     * Devuelve un puntero a la Queue donde llegarán todas las acciones de los jugadores.
     */
    std::shared_ptr<Queue<PlayerAction>> getActionsQueue();

    const Match& getMatch() const;

    void kill();

private:
    void run() override;
};

#endif  // GAMELOOP_H_
