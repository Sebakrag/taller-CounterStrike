/*
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
    if (mediaPlayer && mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
        mediaPlayer->stop();
    }
}

void AudioManager::playMenuMusic() {
    if (!mediaPlayer) return;

    // Verificar si el archivo existe como recurso
    QFile file("client/assets/sfx/music/menu.wav");
    if (!file.exists()) {
        std::cerr << "[AudioManager] ERROR: El archivo de audio NO EXISTE como recurso en la ruta
indicada" << std::endl;
    }

    // Establecer la fuente de la música (Probamos ambas opciones)
    QString resourcePath = "client/assets/sfx/music/menu.wav";

    // Intenta primero con la ruta de recurso
    mediaPlayer->setSource(QUrl(resourcePath));

    // Iniciar reproducción
    mediaPlayer->play();
    musicPlaying = true;

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
*/
