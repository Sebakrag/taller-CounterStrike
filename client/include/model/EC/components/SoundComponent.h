#ifndef SOUNDCOMPONENT_H
#define SOUNDCOMPONENT_H

#include <vector>

#include "../../../../../client/include/model/utils/SoundEvent.h"

class SoundComponent {
private:
    std::vector<SoundEvent> eventsToPlay;

public:
    SoundComponent();

    void addEvent(SoundEvent ev);

    std::vector<SoundEvent>& getEvents();

    bool hasEvents() const;

    void clearEvents();
};

#endif  // SOUNDCOMPONENT_H

// std::unordered_set<SoundEvent> currentlyPlayingLooped; // Sonidos en loop activos

// void markLooped(SoundEvent event) {
//     currentlyPlayingLooped.insert(event);
// }
//
// void unmarkLooped(SoundEvent event) {
//     currentlyPlayingLooped.erase(event);
// }
//
// bool isLoopedPlaying(SoundEvent event) const {
//     return currentlyPlayingLooped.contains(event);
// }
