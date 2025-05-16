#ifndef GAMEMATCHAPPSTATE_H
#define GAMEMATCHAPPSTATE_H

#include "AppState.h"

class GameMatchAppState: public AppState {
private:
    bool handle_events();

public:
    GameMatchAppState();

    std::optional<AppStateCode> update() override;

    ~GameMatchAppState() override;
};

#endif  // GAMEMATCHAPPSTATE_H
