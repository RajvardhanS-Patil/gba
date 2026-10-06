#pragma once
// ═══════════════════════════════════════════════════════════════════════════
// RUNTIME: The Last Algorithm
// AudioManager — Central audio management with separate channels
// ═══════════════════════════════════════════════════════════════════════════

#include "raylib.h"
#include <string>
#include <unordered_map>
#include <memory>

// ─── Volume channels ────────────────────────────────────────────────────────
struct VolumeSettings {
    float master  = 0.8f;
    float music   = 0.7f;
    float sfx     = 0.8f;
    float voice   = 0.9f;
    float ui      = 0.7f;
};

class AudioManager {
public:
    static AudioManager& instance();

    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    // Lifecycle
    void init();
    void shutdown();
    void update();

    // Volume control
    void setMasterVolume(float vol);
    void setMusicVolume(float vol);
    void setSFXVolume(float vol);
    void setVoiceVolume(float vol);
    void setUIVolume(float vol);

    float getMasterVolume() const;
    float getMusicVolume() const;
    float getSFXVolume() const;
    float getVoiceVolume() const;
    float getUIVolume() const;

    VolumeSettings getVolumeSettings() const;
    void setVolumeSettings(const VolumeSettings& settings);

    // Effective volume (channel * master)
    float getEffectiveMusic() const;
    float getEffectiveSFX() const;
    float getEffectiveVoice() const;
    float getEffectiveUI() const;

    // Sound loading
    Sound loadSound(const std::string& filepath);
    Music loadMusic(const std::string& filepath);
    void unloadAll();

    // Quick play (loads if not cached)
    void playSFX(const std::string& filepath);
    void playUI(const std::string& filepath);

    bool isInitialized() const { return initialized_; }

private:
    AudioManager();
    ~AudioManager();

    float clampVolume(float vol) const;

    VolumeSettings volumes_;
    std::unordered_map<std::string, Sound> soundCache_;
    bool initialized_ = false;
};
