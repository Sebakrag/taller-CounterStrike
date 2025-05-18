#include "client/include/app-state/LoginAppState.h"

#include <iostream>  // Eliminar una vez tengamos la parte grafica.

LoginAppState::LoginAppState() {
    // Here we can initialize some music. This applies only if we
    // are using the heap to create the app states.
    // Otherwise, we should override the enter() method of AppState, and
    // there we should initialize the music.
}

// std::optional<AppStateCode> LoginAppState::update() {
// struct state_result_t {
//     std::optional<AppStateCode> app_state_code;
//     Client client;
// };

// state_result_t LoginAppState::update() {
std::optional<AppStateCode> LoginAppState::update() {
    std::optional<AppStateCode> new_app_state;

    // Parte grafica:
    // Crear ventana que tenga un cuadro de texto para que el usuario
    // ingrese su nombre.
    // Y otro cuadro de texto para que ingrese la IP del Server.
    // Con un boton "connect".

    // Con los datos tomando (nombre, y la direccion IP)
    // Client client(dir_ip, port, usr_name);
    std::cout << "Ya me logueeeee!" << std::endl;
    new_app_state = AppStateCode::GAME_MATCH;  // Para pasar directo al juego y probarlo.


    // return {new_app_state, std::move(client)};
    return new_app_state;
}

LoginAppState::~LoginAppState() {
    // Here we should deinitilize all the things we initilized in the constructor.
}
