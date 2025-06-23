#include "client/include/model/Audio.h"

namespace {
constexpr int MAX_CHANNELS_ALLOCATED = 64;
}  // namespace


Audio::Audio():
        sdl_mixer(MIX_INIT_OGG),
        mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) {
    mixer.AllocateChannels(MAX_CHANNELS_ALLOCATED);
}

int Audio::playChannel(const int channel, const Chunk& chunk) {
    return mixer.PlayChannel(channel, chunk);
}

int Audio::playLoopChannel(const int channel, const Chunk& chunk) {
    return mixer.PlayChannel(channel, chunk, -1);  // -1 = loop infinito
}

int Audio::setVolume(const int channel, const int volume) {
    return mixer.SetVolume(channel, volume);
}

void Audio::haltChannel(const int channel) { mixer.HaltChannel(channel); }

void Audio::haltAllChannels() {
    mixer.HaltChannel(-1);  // -1 to halt all channels.
}
