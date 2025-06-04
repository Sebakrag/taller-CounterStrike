#ifndef WEAPONSPRITECOMPONENT_H
#define WEAPONSPRITECOMPONENT_H

#include "common/types.h"

#include "SpriteComponent.h"

class WeaponSpriteComponent: public SpriteComponent {
private:
    WeaponState state;
    // offset para cualquier arma: 18 pixeles (verticalmente).
    // Para que esto sea constante hay que tener cuidado de crear los spritesheets de la forma
    // adecuada. Las armas tiene que encontrarse a 14 pixeles de distancia del margen izquierdo,
    // y dependiendo de si el arma cae justo en la mano o si el jugador debe sostenerla con las
    // 2 manos debemos hacer la cuenta para que siempre el offset sea 18 pixeles.
    Vec2D renderOffset;  // Sirve para renderizar el arma justo arriba de las manos del jugador.

    void setFrameForState(WeaponState state);

public:
    WeaponSpriteComponent(): state(WeaponState::HIDDEN) {}

    void init(SpriteType type, WeaponState initialState);
    void setState(WeaponState newState);
    WeaponState getState() const;
};


#endif  // WEAPONSPRITECOMPONENT_H
