#include "client/include/model/utils/Mouse.h"

#include "client/client_constants.h"
#include "model/Graphics.h"

Mouse::Mouse() {
    if (!texture) {
        throw std::runtime_error("Error: no se pudo cargar la textura del mouse pointer.");
    }
    srcRect.w = MOUSE_POINTER_SPRITE_SIZE;
    srcRect.h = MOUSE_POINTER_SPRITE_SIZE;
    onScreenRect.w = MOUSE_POINTER_SIZE_ON_SCREEN;
    onScreenRect.h = MOUSE_POINTER_SIZE_ON_SCREEN;
    pointer.w = 1;
    pointer.h = 1;
    SDL_ShowCursor(false);
}

void Mouse::update() {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // Posicionamos el rectángulo de la textura centrado en el mouse
    onScreenRect.x = mouseX - onScreenRect.w / 2;
    onScreenRect.y = mouseY - onScreenRect.h / 2;

    // Actualizamos la posición lógica del puntero (para colisiones, disparos, etc.)
    pointer.x = mouseX;
    pointer.y = mouseY;
}

void Mouse::render(Graphics& graphics) { graphics.draw(*texture, srcRect, onScreenRect); }
