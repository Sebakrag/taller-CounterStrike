#include "client/include/model/EC/AudioSystem.h"

#include <unordered_set>

#include <SDL2/SDL_audio.h>

#include "client/include/model/EC/components/SoundComponent.h"
#include "client/include/model/EC/components/TransformComponent.h"

AudioSystem::AudioSystem(ComponentManager& cm, Mixer& m): comp_mgr(cm), mixer(m) {}

// void AudioSystem::update(const Vec2D& listenerPos) {
//     comp_mgr.forEach<SoundComponent>([&](SoundComponent& soundComp, const Entity e) {
//         if (!soundComp.hasEvents())
//             return;
//
//         const auto* transform = comp_mgr.getComponent<TransformComponent>(e);
//         if (!transform)
//             return;  // TODO: En caso de que haya una entidad que pueda producir sonido pero
//                      /// que no tenga posicion, quizas podemos prefijar un sonido a la mitad del
//                      /// maximo del volumen. Por ejemplo: para la radio (que la escuchan todos).
//
//         const Vec2D soundPos = transform->getPosition();
//         if (const int volume = static_cast<int>(calculateVolume(soundPos, listenerPos))) {
//             for (const SoundEvent event: soundComp.getEvents()) {
//                 if (const auto chunk = sound_lib.get(event)) {
//                     const int channel = mixer.PlayChannel(-1, *chunk);  // loops=0 by default.
//                     mixer.SetVolume(channel, volume);
//                 }
//             }
//         }
//
//         soundComp.clearEvents();  // Limpiar para el próximo frame
//     });
// }

void AudioSystem::update(const Vec2D& listenerPos) {
    int soundsPlayed = 0;
    std::unordered_set<Entity> entitiesWithSound;

    comp_mgr.forEach<SoundComponent>([&](SoundComponent& soundComp, const Entity e) {
        entitiesWithSound.insert(e);

        const auto* transform = comp_mgr.getComponent<TransformComponent>(e);
        if (!transform)
            return;
        // TODO: En caso de que haya una entidad que pueda producir sonido pero
        /// que no tenga posicion, quizas podemos prefijar un sonido a la mitad del
        /// maximo del volumen. Por ejemplo: para la radio (que la escuchan todos).

        const Vec2D soundPos = transform->getPosition();
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
                        const int channel =
                                mixer.PlayChannel(-1, *chunk, -1);  // -1 = loop infinito
                        mixer.SetVolume(channel, volume);
                        activeLoops[e][ev] = channel;
                    }
                }
            } else if (soundsPlayed < MAX_SOUNDS_PER_FRAME) {
                // One-shot sound: solo reproducimos si no nos pasamos del límite global
                if (const auto chunk = sound_lib.get(ev)) {
                    const int channel = mixer.PlayChannel(-1, *chunk, 0);
                    mixer.SetVolume(channel, volume);
                    ++soundsPlayed;
                }
            }
        }

        // Detenemos loops que ya no están activos
        auto& prev = activeLoops[e];
        for (auto it = prev.begin(); it != prev.end();) {
            if (!currentLoops.contains(it->first)) {
                mixer.HaltChannel(it->second);
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
            for (auto& [_, ch]: it->second) mixer.HaltChannel(ch);
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
