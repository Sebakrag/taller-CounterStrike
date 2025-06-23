#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2pp/SDL2pp.hh>
using SDL2pp::Chunk;
using SDL2pp::Mixer;
using SDL2pp::SDLMixer;

class Audio {
private:
    SDLMixer sdl_mixer;
    Mixer mixer;
    // Music music;

public:
    Audio();
    int playChannel(int channel, const Chunk& chunk);
    int playLoopChannel(int channel, const Chunk& chunk);
    int setVolume(int channel, int volume);
    void haltChannel(int channel);
    void haltAllChannels();
};

#endif  // AUDIO_H
