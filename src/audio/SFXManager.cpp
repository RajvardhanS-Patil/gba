// ═══════════════════════════════════════════════════════════════════════════
// RUNTIME: The Last Algorithm
// SFXManager — Implementation
// ═══════════════════════════════════════════════════════════════════════════

#include "audio/SFXManager.h"
#include "utils/Logger.h"
#include <filesystem>

SFXManager& SFXManager::instance() {
    static SFXManager inst;
    return inst;
}

SFXManager::SFXManager() = default;

SFXManager::~SFXManager() {
    unloadAll();
}

void SFXManager::preload(const std::string& id, const std::string& filepath) {
    if (!AudioManager::instance().isInitialized()) return;

    if (!std::filesystem::exists(filepath)) {
        LOG_WARN("SFX file not found: " + filepath + " (id: " + id + ")");
        return;
    }

    SFXEntry entry;
    entry.filepath = filepath;
    entry.sound = LoadSound(filepath.c_str());
    entry.loaded = true;
    sounds_[id] = entry;
    LOG_INFO("Preloaded SFX: " + id + " (" + filepath + ")");
}

void SFXManager::play(const std::string& id) {
    if (!AudioManager::instance().isInitialized()) return;

    auto it = sounds_.find(id);
    if (it == sounds_.end()) {
        LOG_WARN("SFX not found: " + id);
        return;
    }

    if (!it->second.loaded) return;

    float vol = AudioManager::instance().getEffectiveSFX();
    SetSoundVolume(it->second.sound, vol);
    PlaySound(it->second.sound);
}

void SFXManager::play(const std::string& id, float pitch) {
    if (!AudioManager::instance().isInitialized()) return;

    auto it = sounds_.find(id);
    if (it == sounds_.end()) {
        LOG_WARN("SFX not found: " + id);
        return;
    }

    if (!it->second.loaded) return;

    float vol = AudioManager::instance().getEffectiveSFX();
    SetSoundVolume(it->second.sound, vol);
    SetSoundPitch(it->second.sound, pitch);
    PlaySound(it->second.sound);
}

void SFXManager::playFile(const std::string& filepath) {
    AudioManager::instance().playSFX(filepath);
}

void SFXManager::stop(const std::string& id) {
    auto it = sounds_.find(id);
    if (it != sounds_.end() && it->second.loaded) {
        StopSound(it->second.sound);
    }
}

bool SFXManager::isPlaying(const std::string& id) const {
    auto it = sounds_.find(id);
    if (it != sounds_.end() && it->second.loaded) {
        return IsSoundPlaying(it->second.sound);
    }
    return false;
}

void SFXManager::unloadAll() {
    for (auto& [id, entry] : sounds_) {
        if (entry.loaded) {
            UnloadSound(entry.sound);
        }
    }
    sounds_.clear();
    LOG_INFO("Unloaded all SFX");
}
