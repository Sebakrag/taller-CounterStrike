#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2pp/SDL2pp.hh>
using SDL2pp::Chunk;
using SDL2pp::Mixer;
using SDL2pp::Music;
using SDL2pp::SDLMixer;

class Audio {
private:
    SDLMixer sdl_mixer;
    Mixer mixer;
    // Esta va a ser la misma musica para todo el juego. (para el shop o para la partida en si)
    Music music;

public:
    Audio();
    // for Chunk
    int playChannel(int channel, const Chunk& chunk, float volumePercentage = 1);
    int playLoopChannel(int channel, const Chunk& chunk, float volumePercentage = 1);
    int setVolume(int channel, int volume);
    void haltChannel(int channel);
    void haltAllChannels();
    // for Music
    void playLoopMusic(float volumePercentage = 1);
    int setMusicVolume(int volume);
    void haltMusic();
    bool isMusicPlaying() const;
};

#endif  // AUDIO_H
