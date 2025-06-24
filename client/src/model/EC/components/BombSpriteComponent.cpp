#include "../../../../../client/include/model/EC/components/BombSpriteComponent.h"

void BombSpriteComponent::init(const SpriteType type, const BombState initialState) {
    SpriteComponent::init(type);
    state = initialState;
    setFrameForState(state, 0);
}

void BombSpriteComponent::setState(const BombState newState, int timeLeft) {
    if (newState == state)
        return;
    state = newState;
    setFrameForState(state, timeLeft);
}

BombState BombSpriteComponent::getState() const { return state; }

Vec2D BombSpriteComponent::getRenderOffset() const { return renderOffset; }

void BombSpriteComponent::setFrameForState(const BombState state, int timeLeft) {
    tickCounter++;
    int row = 0;
    switch (state) {
        case BombState::Dropped:
            row = 0;
            break;
        case BombState::Equipped:
            row = 1;
            break;
        case BombState::Planted:
            row = 2;
            break;
        case BombState::Defused:
            row = 3;
            break;
        case BombState::Exploded:
            row = 4;
            break;               // TODO: Agregar secuencia de explosion.
        case BombState::Hidden:  // No se renderiza
        default:
            return;
    }
    int col = 0;
    if (state == BombState::Planted) {
        //beep cada 2 segundos
        if (timeLeft > 5) {
            if (timeLeft %2 == 0) {
                col = 1;
            }
            else {
                col = 0;
            }
        }
        //beep cada 1 segundo
        else if (timeLeft <= 5) {
            if ((tickCounter / 30) % 2 == 0) {  // Suponiendo 60fps
                col = 2;
            } else {
                col = 0;
            }
        }
    }
    setFrame(row, col);
}
