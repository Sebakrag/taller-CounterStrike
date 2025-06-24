#ifndef MAINMENUAPPSTATE_H
#define MAINMENUAPPSTATE_H

#include "AppState.h"

class MainMenuAppState: public AppState {
public:
    explicit MainMenuAppState(AppStateController* ctrl);

    std::optional<AppStateCode> update() override;

    ~MainMenuAppState() override;
};

#endif  // MAINMENUAPPSTATE_H
