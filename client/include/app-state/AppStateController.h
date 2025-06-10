#ifndef APPSTATECONTROLLER_H
#define APPSTATECONTROLLER_H

#include <memory>

#include "../../../client/include/client.h"

#include "AppState.h"


class AppStateController {
private:
    // AppState& current_state; // try to implement in a way that we only use stack
    AppState* current_state;

    std::unique_ptr<Client> client;


    void transition_to(const AppStateCode& new_state);

public:
    AppStateController();

    void update();

    void setClient(std::unique_ptr<Client> c);

    Client* getClient() const;

    ~AppStateController();
};

#endif  // APPSTATECONTROLLER_H
