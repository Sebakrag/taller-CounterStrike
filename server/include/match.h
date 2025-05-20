#ifndef MATCH_H_
#define MATCH_H_

#include <string>
#include <vector>

#include "types.h"


class Match {
private:
    std::vector<std::string> players;  // por ahora strings solos
public:
    Match();

    std::vector<std::string> getPlayers();

    bool addPlayer(const std::string& username);

    void removePlayer(const std::string& username);

    void processAction(const PlayerAction& action);

    GameInfo generateGameInfo() const;
};

#endif  // MATCH_H_
