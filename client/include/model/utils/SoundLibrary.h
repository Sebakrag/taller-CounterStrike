#ifndef SOUNDLIBRARY_H
#define SOUNDLIBRARY_H

#include <string>
#include <unordered_map>

#include <SDL2pp/Chunk.hh>

#include "SoundEvent.h"

using SDL2pp::Chunk;

class SoundLibrary {
private:
    std::unordered_map<SoundEvent, Chunk> sounds;

    void loadSound(SoundEvent ev, const std::string& filepath);
    void loadPLayerSounds();
    void loadBombSounds();
    void loadRadioSounds();

public:
    SoundLibrary();

    // Chunk& get(SoundEvent ev);
    Chunk* get(SoundEvent ev);
    bool isLoopSound(SoundEvent ev) const;
};

#endif  // SOUNDLIBRARY_H
