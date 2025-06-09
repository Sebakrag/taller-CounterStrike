#ifndef GAMEMATCHAPPSTATE_H
#define GAMEMATCHAPPSTATE_H

#include "../../../client/include/client.h"
#include "../../../common/dtos/MatchInfo.h"

#include "AppState.h"

class GameMatchAppState: public AppState {
private:
    Client* client;

public:
    explicit GameMatchAppState(AppStateController* ctrl);

    std::optional<AppStateCode> update() override;

    ~GameMatchAppState() override;
};

#endif  // GAMEMATCHAPPSTATE_H
