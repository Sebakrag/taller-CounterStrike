#include "../../../../client/include/model/utils/EventHandler.h"

#include "../../../../client/dtos/AimInfo.h"
#include "../../../../common/types.h"
#include "../../../../common/utils/Vec2D.h"

EventHandler::EventHandler(Client& client, World& world, Shop& shop):
        client(client), world(world), shop(shop) {}

void EventHandler::handleEvents(bool& gameIsRunning, const GamePhase gamePhase) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            gameIsRunning = false;
            client.ExitGame();
            return;
        }
        // Si estamos en fase de preparacion y hay shop, solo permitir interacción con la tienda
        if (gamePhase == GamePhase::Preparation) {
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                shop.handleClick(e.button.x, e.button.y);

                if (shop.isBuyButtonClicked(e.button.x, e.button.y)) {
                    ShopSelection selection = shop.getSelectionType();

                    switch (selection) {
                        case ShopSelection::WeaponSelected:
                            // TODO: implementar el metodo de cliente
                            // client.buyWeapon(shop.getSelectedWeapon());
                            break;
                        case ShopSelection::PrimaryAmmoSelected:
                        case ShopSelection::SecondaryAmmoSelected:
                            // TODO: implementar el metodo de cliente
                            // client.buyAmmo(shop.getSelectedAmmoType());
                            break;
                        default:
                            break;
                    }

                    shop.resetSelection();
                }
            }
            return;  // Evitamos manejar cualquier otro evento en fase preparacion
        }

        if (e.type == SDL_KEYDOWN) {  // TODO: cambiar a switch.
            if (e.key.repeat == 0)
                handleKeyDown(e.key.keysym.scancode, gameIsRunning);
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            handleMouseButtonDown(e.button);
        }
    }

    handleKeyboardEvents(gameIsRunning);
    handleMouseEvents(gameIsRunning);
}
// TODO: Unificar funciones de keyboard (No esta saliendo bien cuando presiono ESC)
void EventHandler::handleKeyboardEvents(bool& gameIsRunning) {
    const Uint8* state = SDL_GetKeyboardState(NULL);

    // salir
    if (state[SDL_SCANCODE_ESCAPE])
        gameIsRunning = false;

    Uint32 now = SDL_GetTicks();
    if (now - lastKeyboardPress < DELAY_KEYBOARD_PRESS)
        return;
    lastKeyboardPress = now;

    Vec2D direction(0, 0);

    if (state[SDL_SCANCODE_A]) {
        direction.setX(-1);
    } else if (state[SDL_SCANCODE_D]) {
        direction.setX(1);
    }

    if (state[SDL_SCANCODE_W]) {
        direction.setY(-1);
    } else if (state[SDL_SCANCODE_S]) {
        direction.setY(1);
    }

    if ((direction.getX() != 0) || (direction.getY() != 0)) {
        client.move(direction);
    }
}

void EventHandler::handleKeyDown(SDL_Scancode sc, bool& gameIsRunning) const {
    if (sc == SDL_SCANCODE_ESCAPE)
        gameIsRunning = false;

    // cambiar de arma
    if (sc == SDL_SCANCODE_1) {
        client.changeWeapon(TypeWeapon::Primary);
    } else if (sc == SDL_SCANCODE_2) {
        client.changeWeapon(TypeWeapon::Secondary);
    } else if (sc == SDL_SCANCODE_3) {
        client.changeWeapon(TypeWeapon::Knife);
    } else if (sc == SDL_SCANCODE_4) {
        client.changeWeapon(TypeWeapon::Bomb);
    }
}


void EventHandler::handleMouseButtonDown(const SDL_MouseButtonEvent& b) {
    AimInfo aimInfo = world.getPlayerAimInfo(b.x, b.y);
    if (b.button == SDL_BUTTON_LEFT) {
        client.shoot(aimInfo);
    } else if (b.button == SDL_BUTTON_RIGHT) {
        client.pickUpItem();
    }
}

void EventHandler::handleMouseEvents(const bool gameIsRunning) {
    if (!gameIsRunning) {
        return;
    }

    Uint32 now = SDL_GetTicks();
    if (now - lastMouseProcessTime < DELAY_MOUSE_MOTION)
        return;
    lastMouseProcessTime = now;

    int mouseX, mouseY;
    const Uint32 mouseButtons = SDL_GetMouseState(&mouseX, &mouseY);

    const AimInfo aimInfo = world.getPlayerAimInfo(mouseX, mouseY);
    if (mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT)) {  // si mantiene el click izquierdo
        if (aimInfo.currentWeapon == Weapon::Ak47) {
            client.shoot(aimInfo);
        }
    }
    if (aimInfo.angle != lastAimAngle) {  // Solo rotar si el ángulo realmente cambió
        client.rotate(aimInfo.angle);
        lastAimAngle = aimInfo.angle;
    }
}
