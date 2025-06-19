#include "client/include/model/EC/AudioSystem.h"

#include <SDL2/SDL_audio.h>

#include "client/include/model/EC/components/TransformComponent.h"
#include "client/include/model/EC/components/SoundComponent.h"

AudioSystem::AudioSystem(ComponentManager& cm, Mixer& m, const Entity lp): comp_mgr(cm), mixer(m), local_player(lp) {}

void AudioSystem::update() {
    const auto* localTransform = comp_mgr.getComponent<TransformComponent>(local_player);
    if (!localTransform) return;

    const Vec2D listenerPos = localTransform->getPosition();

    comp_mgr.forEach<SoundComponent>([&](SoundComponent& soundComp, const Entity e) {
        if (!soundComp.hasEvents()) return;

        const auto* transform = comp_mgr.getComponent<TransformComponent>(e);
        if (!transform) return; // TODO: En caso de que haya una entidad que pueda producir sonido pero
                                /// que no tenga posicion, quizas podemos prefijar un sonido a la mitad del maximo del volumen.
                                /// Por ejemplo: para la radio (que la escuchan todos).

        const Vec2D soundPos = transform->getPosition();
        const int volume = static_cast<int>(calculateVolume(soundPos, listenerPos) * MIX_MAX_VOLUME);

        for (const SoundEvent event : soundComp.getEvents()) {
            if (const auto chunk = sound_lib.get(event)) {
                mixer.SetVolume(-1, volume);
                mixer.PlayChannel(-1, *chunk); // loops=0 by default.
            }
            // mixer.SetVolume(-1, volume);
            // mixer.PlayChannel(-1, sound_lib.get(event)); // loops=0 by default.
        }

        soundComp.clearEvents();  // Limpiar para el próximo frame
    });
}

float AudioSystem::calculateVolume(const Vec2D& soundPos, const Vec2D& listenerPos) const {
    const float distSquared = (soundPos - listenerPos).calculateNormSquared();
    if (distSquared >= MAX_SOUND_DISTANCE * MAX_SOUND_DISTANCE)
        return 0.0f;

    // Lineal: volumen máximo si dist = 0, 0 si >= max
    const float dist = std::sqrt(distSquared);
    return 1.0f - (dist / MAX_SOUND_DISTANCE);
}
