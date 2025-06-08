#ifndef GAMEMATCHAPPSTATE_H
#define GAMEMATCHAPPSTATE_H

#include "AppState.h"

class GameMatchAppState: public AppState {
public:
    explicit GameMatchAppState(AppStateController* ctrl);

    std::optional<AppStateCode> update() override;

    ~GameMatchAppState() override;
};

#endif  // GAMEMATCHAPPSTATE_H
