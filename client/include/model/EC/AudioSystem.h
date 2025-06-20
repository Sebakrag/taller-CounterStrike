#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include <unordered_map>

#include "SDL2pp/Mixer.hh"
#include "common/utils/Vec2D.h"
#include "model/utils/SoundLibrary.h"

#include "ComponentManager.h"

constexpr float MAX_SOUND_DISTANCE =
        600.0f;  // TODO: Quizas que lo podemos agregar al archivo de config del server.
constexpr int MAX_SOUNDS_PER_FRAME = 10;  // Limite global de reproducción de sonidos por frame

using SDL2pp::Mixer;

class AudioSystem {
private:
    SoundLibrary sound_lib;
    ComponentManager& comp_mgr;
    Mixer& mixer;  // TODO: Quizas es buena idea encapsular el mixer en un modulo/clase Audio.

    std::unordered_map<Entity, std::unordered_map<SoundEvent, int>> activeLoops;

    float calculateVolume(const Vec2D& soundPos, const Vec2D& listenerPos) const;

public:
    AudioSystem(ComponentManager& cm, Mixer& m);

    void update(const Vec2D& listenerPos);
};

#endif  // AUDIOSYSTEM_H
