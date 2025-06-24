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
    KnifeHit,
    WeaponClick,
    ChangeWeapon,
    Die,
    PickUpWeapon,
    DropWeapon,
    TakeDamage,
    // Radio sounds
    PlantBomb,
    DefuseBomb,
    Go,
    TerroristWin,
    CTWin,

    // Bomb sounds
    BombBeep,
    BombExploded,
};

#endif  // SOUNDEVENT_H
