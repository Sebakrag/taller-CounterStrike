#ifndef SYSTEM_H
#define SYSTEM_H

#include "ComponentManager.h"

class System {
public:
    virtual void update(float delta_time, ComponentManager& cm) = 0;
    virtual ~System() = default;
};


#endif  // SYSTEM_H
