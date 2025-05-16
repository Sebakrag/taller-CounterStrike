#ifndef APPSTATECONTROLLER_H
#define APPSTATECONTROLLER_H

#include "AppState.h"


class AppStateController {
private:
    // AppState& current_state; // try to implement in a way that we only use stack
    AppState* current_state;

    void transition_to(const AppStateCode& new_state);

public:
    AppStateController();

    void update();

    ~AppStateController();
};

#endif  // APPSTATECONTROLLER_H
