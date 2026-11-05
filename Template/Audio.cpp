#include "Audio.h"
#include <iostream>
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"



//shared audio engine
static ma_engine audioEngine;
static bool audioInitialized = false;

bool initAudio() {
    if (audioInitialized)
        return true;

    if (ma_engine_init(NULL, &audioEngine) != MA_SUCCESS) {
        std::cerr << "Failed to initialize audio engine\n";
        return false;
    }

    audioInitialized = true;
    return true;
}

void shutdownAudio() {
    if (!audioInitialized)
        return;

    ma_engine_uninit(&audioEngine);
    audioInitialized = false;
}

void playSound(const char* filePath) {
    if (!audioInitialized || filePath == nullptr)
        return;

    if (ma_engine_play_sound(&audioEngine, filePath, NULL) != MA_SUCCESS) {
        std::cerr << "Failed to play sound: " << filePath << '\n';
    }
}

void setAudioVolume(float volume, bool muted) {
    if (!audioInitialized)
        return;

    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;

    ma_engine_set_volume(
        &audioEngine,
        muted ? 0.0f : volume
    );
}