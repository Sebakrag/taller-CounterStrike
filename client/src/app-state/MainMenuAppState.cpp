#include "client/include/app-state/MainMenuAppState.h"

#include <iostream>  // Elimar una vez que tengamos la parte grafica con QT.

MainMenuAppState::MainMenuAppState() {
    // inicilizar musica
}

std::optional<AppStateCode> MainMenuAppState::update() {
    std::optional<AppStateCode> new_app_state;

    // Aca deberiamos tener el codigo de SDL correspondiente a la ventana del menu principal.
    // donde tendriamos el listado de botones con las siguientes opciones:
    // - Crear Partida
    // - Unirse Partida
    // - Salir
    // Obvio que vamos a tener que atrapar los eventos y manejarlos segun corresponda.
    std::cout << "Volvi al menu!" << std::endl;
    new_app_state = AppStateCode::QUIT;

    return new_app_state;
}

MainMenuAppState::~MainMenuAppState() {
    // deinicializar musica
}
