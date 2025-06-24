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
        // Manejo global de ESC (aplicable a cualquier fase)
        if (e.type == SDL_KEYDOWN && e.key.repeat == 0 &&
            e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            gameIsRunning = false;
            client.ExitGame();
            return;
        }
        switch (gamePhase) {
            case GamePhase::Preparation:
                handlePreparationPhaseEvents(e);
                break;
            case GamePhase::Combat:
                handleCombatPhaseEvents(e);
                break;
            default:
                break;
        }
    }
    // Una sola vez por frame
    if (gamePhase == GamePhase::Combat) {
        handleKeyboardEvents(gameIsRunning);
        handleMouseEvents(gameIsRunning);
    }
}

void EventHandler::handlePreparationPhaseEvents(const SDL_Event& e) const {
    // En la fase de preparacion solo permitimos interaccion con la tienda.
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (shop.isBuyButtonClicked(e.button.x, e.button.y)) {
            switch (shop.getSelectionType()) {
                case ShopSelection::WeaponSelected:
                    client.buyWeapon(shop.getSelectedWeapon());
                    return;
                case ShopSelection::PrimaryAmmoSelected:
                case ShopSelection::SecondaryAmmoSelected:
                    client.buyAmmo(shop.getSelectedAmmoType());
                    return;
                default:
                    return;
            }
        }
        shop.handleClick(e.button.x, e.button.y);
    }
}

void EventHandler::handleCombatPhaseEvents(const SDL_Event& e) const {
    switch (e.type) {
        case SDL_KEYDOWN:
            if (e.key.repeat == 0)
                handleKeyDown(e.key.keysym.scancode);
            break;
        case SDL_MOUSEBUTTONDOWN:
            handleMouseButtonDown(e.button);
            break;
        default:
            break;
    }
}

void EventHandler::handleKeyDown(const SDL_Scancode sc) const {
    switch (sc) {
        case SDL_SCANCODE_1:
            client.changeWeapon(TypeWeapon::Primary);
            break;
        case SDL_SCANCODE_2:
            client.changeWeapon(TypeWeapon::Secondary);
            break;
        case SDL_SCANCODE_3:
            client.changeWeapon(TypeWeapon::Knife);
            break;
        case SDL_SCANCODE_4:
            client.changeWeapon(TypeWeapon::Bomb);
            break;
        case SDL_SCANCODE_R:
            client.defuseBomb();
            break;
        default:
            break;
    }
}

void EventHandler::handleMouseButtonDown(const SDL_MouseButtonEvent& b) const {
    AimInfo aimInfo = world.getPlayerAimInfo(b.x, b.y);
    if (b.button == SDL_BUTTON_LEFT) {
        client.shoot(aimInfo);
    } else if (b.button == SDL_BUTTON_RIGHT) {
        client.pickUpItem();
    }
}

void EventHandler::handleKeyboardEvents(bool& gameIsRunning) {
    const Uint8* state = SDL_GetKeyboardState(NULL);

    // salir
    if (state[SDL_SCANCODE_ESCAPE] || !gameIsRunning) {
        gameIsRunning = false;
        client.ExitGame();
        return;
    }

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
    if (mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT) &&
        isAutomatic(aimInfo.currentWeapon)) {  // si mantiene el click izquierdo
        client.shoot(aimInfo);
    }
    if (aimInfo.angle != lastAimAngle) {  // Solo rotar si el ángulo realmente cambió
        client.rotate(aimInfo.angle);
        lastAimAngle = aimInfo.angle;
    }
}

// TODO: Esta funcion deberia ser utilitaria (no deberia estar en EventHandler).
bool EventHandler::isAutomatic(const Weapon weapon) const { return weapon == Weapon::Ak47; }
