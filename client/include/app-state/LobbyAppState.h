#ifndef LOBBYAPPSTATE_H
#define LOBBYAPPSTATE_H

#include "AppState.h"

class LobbyAppState: public AppState {
public:
    explicit LobbyAppState(AppStateController* ctrl);

    std::optional<AppStateCode> update() override;

    ~LobbyAppState() override;
};

#endif  // LOBBYAPPSTATE_H