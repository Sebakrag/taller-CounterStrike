#ifndef ENTITYTYPE_H
#define ENTITYTYPE_H

enum class EntityType : unsigned char {
    PLAYER = 1,
    WEAPON,
    BULLET,
    BOMB,
    DROP  // item
};

#endif  // ENTITYTYPE_H
