#ifndef AUDIO_MANAGER_H_
#define AUDIO_MANAGER_H_

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QString>
#include <string>
#include <memory>

class AudioManager {
public:
    static AudioManager& getInstance();

    // Prohibir copias y asignaciones para mantener el singleton
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    // Iniciar reproducción de música de menú
    void playMenuMusic();

    // Detener música
    void stopMusic();

    // Ajustar volumen (0.0 a 1.0)
    void setVolume(float volume);

    // Verificar si la música está reproduciéndose
    bool isPlaying() const;

private:
    // Constructor privado (singleton)
    AudioManager();
    ~AudioManager();

    std::unique_ptr<QMediaPlayer> mediaPlayer;
    std::unique_ptr<QAudioOutput> audioOutput;
    bool musicPlaying;
};

#endif  // AUDIO_MANAGER_H_
