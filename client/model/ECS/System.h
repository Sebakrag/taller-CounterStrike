#ifndef SYSTEM_H
#define SYSTEM_H

#include "ComponentManager.h"

class System {
public:
    virtual ~System() = default;
    virtual void update(float delta_time, ComponentManager& cm) = 0;
};


#endif  // SYSTEM_H
