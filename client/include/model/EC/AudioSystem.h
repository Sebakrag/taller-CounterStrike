#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include <unordered_map>

#include "client/include/model/Audio.h"
#include "client/include/model/utils/SoundLibrary.h"
#include "common/utils/Vec2D.h"

#include "ComponentManager.h"

class AudioSystem {
private:
    SoundLibrary sound_lib;
    ComponentManager& comp_mgr;
    Audio& audio;

    std::unordered_map<Entity, std::unordered_map<SoundEvent, int>> activeLoops;

    float calculateVolume(const Vec2D& soundPos, const Vec2D& listenerPos) const;

public:
    AudioSystem(ComponentManager& cm, Audio& m);

    void update(const Vec2D& listenerPos);
};

#endif  // AUDIOSYSTEM_H
