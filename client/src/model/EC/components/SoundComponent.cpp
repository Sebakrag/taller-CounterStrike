#include "../../../../../client/include/model/EC/components/SoundComponent.h"

#include <algorithm>

#define DEFAULT_NUM_ENTT_SOUNDS_PER_FRAME 5

SoundComponent::SoundComponent() { eventsToPlay.reserve(DEFAULT_NUM_ENTT_SOUNDS_PER_FRAME); }

void SoundComponent::addEvent(const SoundEvent ev) {
    if (std::find(eventsToPlay.begin(), eventsToPlay.end(), ev) == eventsToPlay.end()) {
        eventsToPlay.push_back(ev);
    }
}

std::vector<SoundEvent>& SoundComponent::getEvents() { return eventsToPlay; }

bool SoundComponent::hasEvents() const { return !eventsToPlay.empty(); }

void SoundComponent::clearEvents() { eventsToPlay.clear(); }
