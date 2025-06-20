#ifndef SOUNDCOMPONENT_H
#define SOUNDCOMPONENT_H

#include <unordered_set>
#include <vector>

#include "client/include/model/utils/SoundEvent.h"

class SoundComponent {
private:
    std::vector<SoundEvent> eventsToPlay;
    // std::unordered_set<SoundEvent> currentlyPlayingLooped; // Sonidos en loop activos

public:
    SoundComponent();

    void addEvent(SoundEvent ev);

    std::vector<SoundEvent>& getEvents();

    bool hasEvents() const;

    void clearEvents();

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
};

#endif  // SOUNDCOMPONENT_H
