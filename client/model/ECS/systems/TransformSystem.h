#ifndef TRANSFORMSYSTEM_H
#define TRANSFORMSYSTEM_H
#include "client/model/ECS/System.h"

class TransformSystem : public System {
public:
    TransformSystem() = default;

    void update(float delta_time, ComponentManager& cm) override;
};

#endif //TRANSFORMSYSTEM_H
