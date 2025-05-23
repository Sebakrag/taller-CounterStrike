#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "client/include/model/utils/Vec2D.h"

struct InputComponent {
public:
    Vec2D move_dir;
    bool shooting = false;
    bool switch_weapon = false;

    InputComponent() = default;

    void reset() {
        move_dir.set(0, 0);
        shooting = false;
        switch_weapon = false;
    }
};

#endif  // INPUTCOMPONENT_H
