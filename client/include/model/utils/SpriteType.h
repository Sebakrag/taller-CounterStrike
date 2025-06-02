#ifndef SPRITETYPE_H
#define SPRITETYPE_H

// unsigned char == byte --> This is to make the SpriteType as tiny as possible.
enum class SpriteType : unsigned char {
    // Players sprite types:
    PHEONIX = 1,
    L337_KREW,
    ARTIC_AVENGER,
    GUERRILLA,
    SEAL_FORCE,
    GERMAN_GSG_9,
    UK_SAS,
    FRENCH_GIGN,


    // bullets...
    BULLET,

    // Weapons (en mano)
    //...

    // drops...
    DROP_GLOCK,
    DROP_AK47,
    DROP_M3,
    DROP_AWP,
    DROP_BOMB,

    // Maps sprite types:
    DESERT_MAP,
    AZTEC_MAP,
    TRAINING_MAP,
};

#endif  // SPRITETYPE_H
