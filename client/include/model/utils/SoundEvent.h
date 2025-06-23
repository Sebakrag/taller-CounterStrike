#ifndef SOUNDEVENT_H
#define SOUNDEVENT_H

enum class SoundEvent {
    None,
    // Player sounds
    Walk,
    // Shoot,//sacar shoot solo.
    ShootAK47,
    ShootAWP,
    ShootM3,
    ShootGlock,
    KnifeSlash,
    ChangeWeapon,
    Die,
    PickUpWeapon,
    DropWeapon,
    TakeDamage,
    // Radio sounds
    PlantBomb,
    DefuseBomb,
    // Bomb sounds
    BombBeep,
    BombExploded,
};

#endif  // SOUNDEVENT_H
