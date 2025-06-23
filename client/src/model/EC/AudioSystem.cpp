#include "client/include/model/EC/AudioSystem.h"

#include <unordered_set>

#include <SDL2/SDL_audio.h>

#include "client/include/model/EC/components/SoundComponent.h"
#include "client/include/model/EC/components/TransformComponent.h"

namespace {
constexpr float MAX_SOUND_DISTANCE =
        600.0f;  // TODO: Quizas que lo podemos agregar al archivo de config del server.
constexpr int MAX_SOUNDS_PER_FRAME = 10;  // Limite global de reproducción de sonidos por frame
}  // namespace


AudioSystem::AudioSystem(ComponentManager& cm, Audio& m): comp_mgr(cm), audio(m) {}

void AudioSystem::update(const Vec2D& listenerPos) {
    int soundsPlayed = 0;
    std::unordered_set<Entity> entitiesWithSound;

    comp_mgr.forEach<SoundComponent>([&](SoundComponent& soundComp, const Entity e) {
        entitiesWithSound.insert(e);

        const auto* transform = comp_mgr.getComponent<TransformComponent>(e);
        const Vec2D soundPos = transform ? transform->getPosition() : listenerPos;

        const int volume = static_cast<int>(calculateVolume(soundPos, listenerPos));
        if (volume == 0)
            return;

        std::unordered_set<SoundEvent> currentLoops;

        for (const SoundEvent ev: soundComp.getEvents()) {
            if (sound_lib.isLoopSound(ev)) {
                currentLoops.insert(ev);

                // Si aun no estaba reproduciendose, lo arrancamos en loop
                auto it = activeLoops[e].find(ev);
                if (it == activeLoops[e].end()) {
                    if (const auto chunk = sound_lib.get(ev)) {
                        const int channel = audio.playLoopChannel(-1, *chunk);
                        audio.setVolume(channel, volume);
                        activeLoops[e][ev] = channel;
                    }
                }
            } else if (soundsPlayed < MAX_SOUNDS_PER_FRAME) {
                // One-shot sound: solo reproducimos si no nos pasamos del límite global
                if (const auto chunk = sound_lib.get(ev)) {
                    const int channel = audio.playChannel(-1, *chunk);
                    audio.setVolume(channel, volume);
                    ++soundsPlayed;
                }
            }
        }

        // Detenemos loops que ya no están activos
        auto& prev = activeLoops[e];
        for (auto it = prev.begin(); it != prev.end();) {
            if (!currentLoops.contains(it->first)) {
                audio.haltChannel(it->second);
                it = prev.erase(it);
            } else {
                ++it;
            }
        }

        soundComp.clearEvents();
    });

    // Limpiamos entidades que ya no existen
    for (auto it = activeLoops.begin(); it != activeLoops.end();) {
        if (!entitiesWithSound.contains(it->first)) {
            for (auto& [_, ch]: it->second) audio.haltChannel(ch);
            it = activeLoops.erase(it);
        } else {
            ++it;
        }
    }
}

float AudioSystem::calculateVolume(const Vec2D& soundPos, const Vec2D& listenerPos) const {
    const float distSquared = (soundPos - listenerPos).calculateNormSquared();
    if (distSquared >= MAX_SOUND_DISTANCE * MAX_SOUND_DISTANCE)
        return 0.0f;

    // Lineal: volumen máximo si dist = 0, 0 si >= max
    const float dist = std::sqrt(distSquared);
    return (1.0f - (dist / MAX_SOUND_DISTANCE)) * MIX_MAX_VOLUME;
}
