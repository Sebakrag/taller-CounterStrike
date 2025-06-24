
#include "../../../client/include/ui/AudioManager.h"

#include <QAudioOutput>
#include <QFile>
#include <QMediaPlayer>
#include <QUrl>
#include <iostream>

// Singleton instance
AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

AudioManager::AudioManager() : musicPlaying(false) {
    // Inicializar el reproductor de medios
    mediaPlayer = std::make_unique<QMediaPlayer>();
    audioOutput = std::make_unique<QAudioOutput>();

    // Configura el dispositivo de salida de audio para el reproductor
    mediaPlayer->setAudioOutput(audioOutput.get());

    // Configura un volumen máximo
    audioOutput->setVolume(1.0);

    // Configura la reproducción en bucle
    mediaPlayer->setLoops(QMediaPlayer::Infinite);
}

AudioManager::~AudioManager() {
    // Detener la reproducción antes de destruir
    if (mediaPlayer) {
        mediaPlayer->stop();
    }
    
    // Asegurarse de que audioOutput se destruya después de que mediaPlayer deje de usarlo
    mediaPlayer.reset();
    audioOutput.reset();
}

void AudioManager::playMenuMusic() {
    if (!mediaPlayer || !audioOutput) return;

    // Detener cualquier reproducción actual primero
    mediaPlayer->stop();

    // Verificar si el archivo existe
    QString resourcePath = "client/assets/sfx/music/menu.wav";
    QFile file(resourcePath);
    if (!file.exists()) {
        std::cerr << "[AudioManager] ERROR: El archivo de audio no existe en: " << resourcePath.toStdString() << std::endl;
        return; // No continuar si el archivo no existe
    }

    // Establecer la fuente usando QUrl::fromLocalFile para archivos locales
    mediaPlayer->setSource(QUrl::fromLocalFile(resourcePath));

    // Verificar que la fuente se haya establecido correctamente
    if (mediaPlayer->source().isEmpty()) {
        std::cerr << "[AudioManager] ERROR: No se pudo establecer la fuente de audio" << std::endl;
        return;
    }

    // Iniciar reproducción
    mediaPlayer->play();
    musicPlaying = (mediaPlayer->playbackState() == QMediaPlayer::PlayingState);
}

void AudioManager::stopMusic() {
    if (mediaPlayer) {
        mediaPlayer->stop();
        musicPlaying = false;
    }
}

void AudioManager::setVolume(float volume) {
    if (audioOutput) {
        audioOutput->setVolume(volume);
    }
}

bool AudioManager::isPlaying() const {
    return musicPlaying && mediaPlayer &&
           mediaPlayer->playbackState() == QMediaPlayer::PlayingState;
}

