#ifndef GAMEMATCHAPPSTATE_H
#define GAMEMATCHAPPSTATE_H

#include "AppState.h"

class GameMatchAppState: public AppState {
public:
    GameMatchAppState();

    std::optional<AppStateCode> update() override;

    ~GameMatchAppState() override;
};

#endif  // GAMEMATCHAPPSTATE_H
