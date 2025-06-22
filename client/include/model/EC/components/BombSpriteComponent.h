#ifndef BOMBSPRITECOMPONENT_H
#define BOMBSPRITECOMPONENT_H

#include "../../../../../common/types.h"

#include "SpriteComponent.h"

#define HORIZONTAL_RENDER_OFFSET 17
#define VERTICAL_RENDER_OFFSET 0

class BombSpriteComponent: public SpriteComponent {
private:
    BombState state;
    Vec2D renderOffset;  // Sirve para renderizar el arma justo arriba de las manos del jugador.

    void setFrameForState(BombState state);

public:
    BombSpriteComponent():
            state(BombState::Hidden),
            renderOffset(HORIZONTAL_RENDER_OFFSET, VERTICAL_RENDER_OFFSET) {}

    void init(SpriteType type, BombState initialState);
    void setState(BombState newState);
    BombState getState() const;
    Vec2D getRenderOffset() const;
};


#endif  // BOMBSPRITECOMPONENT_H
