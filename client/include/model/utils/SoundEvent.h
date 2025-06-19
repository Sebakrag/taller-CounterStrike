#ifndef SOUNDEVENT_H
#define SOUNDEVENT_H

enum class SoundEvent {
    None,
    // Player sounds
    Walk,
    Shoot,
    ChangeWeapon,
    Die,
    PickUpWeapon,
    PlantBomb,
    DefuseBomb,
    // Bomb sounds
    BombBeep,
    BombExploded,
};

#endif //SOUNDEVENT_H
