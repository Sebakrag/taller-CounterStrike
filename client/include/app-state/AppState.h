#ifndef APPSTATE_H
#define APPSTATE_H

#include <optional>

#include "AppStateCode.h"

class AppStateController;  // Forward declaration

class AppState {
protected:
    AppStateController* controller = nullptr;

public:
    virtual ~AppState() = default;

    // Quizas que si aprovechamos las ventajas de RAII en las clases concretas de AppState
    // podemos eliminar enter y leave. A menos que querramos crear y eliminar efectos de musica
    // cada vez que entremos a un estado.
    // virtual void enter() = 0;
    // virtual void leave() = 0;
    virtual std::optional<AppStateCode> update() = 0;
};

#endif  // APPSTATE_H
