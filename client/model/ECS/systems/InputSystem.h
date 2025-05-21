#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include "client/model/ECS/System.h"


/// \brief Responsible for take the info of the InputComponent of each entity and
/// place it in each component respectively.
class InputSystem : public System {
public:
    InputSystem() = default;

    void update(float delta_time, ComponentManager& cm) override;
};

#endif //INPUTSYSTEM_H
