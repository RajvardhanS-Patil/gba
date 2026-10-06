// ═══════════════════════════════════════════════════════════════════════════
// RUNTIME: The Last Algorithm
// AudioManager — Implementation
// ═══════════════════════════════════════════════════════════════════════════

#include "audio/AudioManager.h"
#include "utils/Logger.h"
#include <algorithm>
#include <filesystem>

AudioManager& AudioManager::instance() {
    static AudioManager inst;
    return inst;
}

AudioManager::AudioManager() = default;

AudioManager::~AudioManager() {
    if (initialized_) {
        shutdown();
    }
}

void AudioManager::init() {
    if (initialized_) return;

    InitAudioDevice();

    if (IsAudioDeviceReady()) {
        LOG_INFO("Audio device initialized successfully");
        initialized_ = true;
    } else {
        LOG_ERROR("Failed to initialize audio device");
    }
}

void AudioManager::shutdown() {
    if (!initialized_) return;

    unloadAll();
    CloseAudioDevice();
    initialized_ = false;
    LOG_INFO("Audio device shut down");
}

void AudioManager::update() {
    // Update any streaming music, etc.
    // Raylib handles most of this internally
}

// ─── Volume control ─────────────────────────────────────────────────────────

float AudioManager::clampVolume(float vol) const {
    return std::clamp(vol, 0.0f, 1.0f);
}

void AudioManager::setMasterVolume(float vol) {
    volumes_.master = clampVolume(vol);
    SetMasterVolume(volumes_.master);
}

void AudioManager::setMusicVolume(float vol)  { volumes_.music = clampVolume(vol); }
void AudioManager::setSFXVolume(float vol)    { volumes_.sfx = clampVolume(vol); }
void AudioManager::setVoiceVolume(float vol)  { volumes_.voice = clampVolume(vol); }
void AudioManager::setUIVolume(float vol)     { volumes_.ui = clampVolume(vol); }

float AudioManager::getMasterVolume() const { return volumes_.master; }
float AudioManager::getMusicVolume() const  { return volumes_.music; }
float AudioManager::getSFXVolume() const    { return volumes_.sfx; }
float AudioManager::getVoiceVolume() const  { return volumes_.voice; }
float AudioManager::getUIVolume() const     { return volumes_.ui; }

VolumeSettings AudioManager::getVolumeSettings() const { return volumes_; }

void AudioManager::setVolumeSettings(const VolumeSettings& settings) {
    volumes_ = settings;
    volumes_.master = clampVolume(volumes_.master);
    volumes_.music  = clampVolume(volumes_.music);
    volumes_.sfx    = clampVolume(volumes_.sfx);
    volumes_.voice  = clampVolume(volumes_.voice);
    volumes_.ui     = clampVolume(volumes_.ui);
    SetMasterVolume(volumes_.master);
}

float AudioManager::getEffectiveMusic() const { return volumes_.master * volumes_.music; }
float AudioManager::getEffectiveSFX() const   { return volumes_.master * volumes_.sfx; }
float AudioManager::getEffectiveVoice() const { return volumes_.master * volumes_.voice; }
float AudioManager::getEffectiveUI() const    { return volumes_.master * volumes_.ui; }

// ─── Sound loading ──────────────────────────────────────────────────────────

Sound AudioManager::loadSound(const std::string& filepath) {
    // Check cache
    auto it = soundCache_.find(filepath);
    if (it != soundCache_.end()) {
        return it->second;
    }

    // Check file exists
    if (!std::filesystem::exists(filepath)) {
        LOG_WARN("Sound file not found: " + filepath);
        return Sound{};
    }

    Sound sound = LoadSound(filepath.c_str());
    soundCache_[filepath] = sound;
    LOG_INFO("Loaded sound: " + filepath);
    return sound;
}

Music AudioManager::loadMusic(const std::string& filepath) {
    if (!std::filesystem::exists(filepath)) {
        LOG_WARN("Music file not found: " + filepath);
        return Music{};
    }

    Music music = LoadMusicStream(filepath.c_str());
    LOG_INFO("Loaded music: " + filepath);
    return music;
}

void AudioManager::unloadAll() {
    for (auto& [path, sound] : soundCache_) {
        UnloadSound(sound);
    }
    soundCache_.clear();
    LOG_INFO("Unloaded all cached sounds");
}

void AudioManager::playSFX(const std::string& filepath) {
    if (!initialized_) return;

    Sound sound = loadSound(filepath);
    if (sound.frameCount > 0) {
        SetSoundVolume(sound, getEffectiveSFX());
        PlaySound(sound);
    }
}

void AudioManager::playUI(const std::string& filepath) {
    if (!initialized_) return;

    Sound sound = loadSound(filepath);
    if (sound.frameCount > 0) {
        SetSoundVolume(sound, getEffectiveUI());
        PlaySound(sound);
    }
}
