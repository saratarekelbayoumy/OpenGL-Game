#pragma once

bool initAudio();

void shutdownAudio();

void playSound(const char* filePath);

void setAudioVolume(float volume, bool muted);