#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "client/model/utils/Vec2D.h"

struct InputComponent {
public:
    Vec2D move_dir;
    bool shooting = false;
    bool switch_weapon = false;

    InputComponent() = default;

    void reset() {
        move_dir.reset();
        shooting = false;
        switch_weapon = false;
    }
};

#endif  // INPUTCOMPONENT_H
