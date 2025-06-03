#ifndef APPSTATECONTROLLER_H
#define APPSTATECONTROLLER_H

#include "AppState.h"
#include <memory>
#include "../client.h"


class AppStateController {
private:
    // AppState& current_state; // try to implement in a way that we only use stack
    AppState* current_state;

    std::shared_ptr<Client> client;

    void transition_to(const AppStateCode& new_state);

public:
    AppStateController();

    void update();

    void setClient(std::shared_ptr<Client> c);
    
    Client* getClient() const;

    ~AppStateController();
};

#endif  // APPSTATECONTROLLER_H
