#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

struct InputComponent {
    bool shooting = false;
    bool switch_weapon = false;
    float x;
    float y;
    // Vec2D move_dir = {0.0f, 0.0f}; // Vec2D va a ser mi propia implementacion de un vector 2D

    InputComponent() = default;
};

#endif  // INPUTCOMPONENT_H
