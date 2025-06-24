#include "client/include/model/Audio.h"

#include "client/client_constants.h"

namespace {
constexpr int MAX_CHANNELS_ALLOCATED = 64;
}  // namespace


Audio::Audio():
        sdl_mixer(MIX_INIT_OGG),
        mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096),
        music(SHOP_MUSIC_FILE_NAME) {
    mixer.AllocateChannels(MAX_CHANNELS_ALLOCATED);
}

int Audio::playChannel(const int channel, const Chunk& chunk, const float volumePercentage) {
    const int ch = mixer.PlayChannel(channel, chunk);
    mixer.SetVolume(ch, volumePercentage * MIX_MAX_VOLUME);
    return ch;
}

int Audio::playLoopChannel(const int channel, const Chunk& chunk, const float volumePercentage) {
    const int ch = mixer.PlayChannel(channel, chunk, -1);  // -1 = loop infinito
    mixer.SetVolume(ch, volumePercentage * MIX_MAX_VOLUME);
    return ch;
}

int Audio::setVolume(const int channel, const int volume) {
    return mixer.SetVolume(channel, volume);
}

void Audio::haltChannel(const int channel) { mixer.HaltChannel(channel); }

void Audio::haltAllChannels() {
    mixer.HaltChannel(-1);  // -1 to halt all channels.
}

void Audio::playLoopMusic(const float volumePercentage) {
    setMusicVolume(MIX_MAX_VOLUME * volumePercentage);
    mixer.PlayMusic(music, -1);  // -1 = loop infinito
}

int Audio::setMusicVolume(const int volume) { return mixer.SetMusicVolume(volume); }

void Audio::haltMusic() { mixer.HaltMusic(); }

bool Audio::isMusicPlaying() const { return mixer.IsMusicPlaying(); }
