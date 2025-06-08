#ifndef LOGINAPPSTATE_H
#define LOGINAPPSTATE_H

#include "AppState.h"

class LoginAppState: public AppState {
public:
    explicit LoginAppState(AppStateController* ctrl);

    std::optional<AppStateCode> update() override;

    ~LoginAppState() override;
};


#endif  // LOGINAPPSTATE_H
