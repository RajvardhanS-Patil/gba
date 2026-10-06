// ═══════════════════════════════════════════════════════════════════════════
// RUNTIME: The Last Algorithm
// VoiceManager — Implementation
// ═══════════════════════════════════════════════════════════════════════════

#include "audio/VoiceManager.h"
#include "utils/Logger.h"
#include <filesystem>

VoiceManager& VoiceManager::instance() {
    static VoiceManager inst;
    return inst;
}

VoiceManager::VoiceManager() = default;

VoiceManager::~VoiceManager() {
    unloadAll();
}

std::string VoiceManager::resolveVoicePath(const std::string& voiceId) const {
    // Voice ID format: "speaker_line_number" e.g., "nova_memory_001"
    // Extract speaker from prefix (everything before first underscore)
    size_t firstUnderscore = voiceId.find('_');
    if (firstUnderscore == std::string::npos) {
        return basePath_ + voiceId + ".ogg";
    }

    std::string speaker = voiceId.substr(0, firstUnderscore);

    // Try multiple formats
    std::vector<std::string> candidates = {
        basePath_ + speaker + "/" + voiceId + ".ogg",
        basePath_ + speaker + "/" + voiceId + ".wav",
        basePath_ + speaker + "/" + voiceId + ".mp3",
        basePath_ + voiceId + ".ogg",
        basePath_ + voiceId + ".wav",
    };

    for (const auto& path : candidates) {
        if (std::filesystem::exists(path)) {
            return path;
        }
    }

    // Return the most likely path even if it doesn't exist
    return basePath_ + speaker + "/" + voiceId + ".ogg";
}

void VoiceManager::play(const std::string& voiceId) {
    if (!AudioManager::instance().isInitialized()) return;

    // Stop current voice
    if (playing_) {
        stop();
    }

    std::string path = resolveVoicePath(voiceId);

    // Check cache
    auto it = voiceCache_.find(voiceId);
    if (it != voiceCache_.end() && it->second.loaded) {
        float vol = AudioManager::instance().getEffectiveVoice();
        SetSoundVolume(it->second.sound, vol);
        PlaySound(it->second.sound);
        currentVoiceId_ = voiceId;
        playing_ = true;
        LOG_INFO("Playing voice (cached): " + voiceId);
        return;
    }

    // Try to load
    if (!std::filesystem::exists(path)) {
        LOG_WARN("Voice file not found: " + path + " (id: " + voiceId + ")");
        // Graceful fallback — just don't play voice, subtitle will still show
        currentVoiceId_ = voiceId;
        playing_ = false;
        return;
    }

    VoiceEntry entry;
    entry.sound = LoadSound(path.c_str());
    entry.loaded = true;
    voiceCache_[voiceId] = entry;

    float vol = AudioManager::instance().getEffectiveVoice();
    SetSoundVolume(entry.sound, vol);
    PlaySound(entry.sound);

    currentVoiceId_ = voiceId;
    playing_ = true;
    LOG_INFO("Playing voice: " + voiceId + " (" + path + ")");
}

void VoiceManager::stop() {
    if (!currentVoiceId_.empty()) {
        auto it = voiceCache_.find(currentVoiceId_);
        if (it != voiceCache_.end() && it->second.loaded) {
            StopSound(it->second.sound);
        }
    }
    playing_ = false;
    currentVoiceId_.clear();
}

void VoiceManager::skip() {
    stop();
    if (onComplete_) {
        onComplete_();
    }
}

bool VoiceManager::isPlaying() const {
    if (!playing_ || currentVoiceId_.empty()) return false;

    auto it = voiceCache_.find(currentVoiceId_);
    if (it != voiceCache_.end() && it->second.loaded) {
        return IsSoundPlaying(it->second.sound);
    }
    return false;
}

std::string VoiceManager::getCurrentVoiceId() const {
    return currentVoiceId_;
}

void VoiceManager::update() {
    if (playing_ && !currentVoiceId_.empty()) {
        auto it = voiceCache_.find(currentVoiceId_);
        if (it != voiceCache_.end() && it->second.loaded) {
            if (!IsSoundPlaying(it->second.sound)) {
                // Voice finished
                playing_ = false;
                if (onComplete_) {
                    onComplete_();
                }
            }
        }
    }
}

void VoiceManager::setOnComplete(std::function<void()> callback) {
    onComplete_ = std::move(callback);
}

void VoiceManager::preloadCharacter(const std::string& character) {
    std::string charPath = basePath_ + character + "/";
    if (!std::filesystem::exists(charPath)) {
        LOG_WARN("Voice directory not found: " + charPath);
        return;
    }

    int count = 0;
    for (const auto& entry : std::filesystem::directory_iterator(charPath)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            if (ext == ".ogg" || ext == ".wav" || ext == ".mp3") {
                std::string id = entry.path().stem().string();
                VoiceEntry ve;
                ve.sound = LoadSound(entry.path().c_str());
                ve.loaded = true;
                voiceCache_[id] = ve;
                count++;
            }
        }
    }
    LOG_INFO("Preloaded " + std::to_string(count) + " voice files for: " + character);
}

void VoiceManager::unloadCharacter(const std::string& character) {
    std::string prefix = character + "_";
    std::vector<std::string> toRemove;

    for (auto& [id, entry] : voiceCache_) {
        if (id.substr(0, prefix.length()) == prefix) {
            if (entry.loaded) {
                UnloadSound(entry.sound);
            }
            toRemove.push_back(id);
        }
    }

    for (const auto& id : toRemove) {
        voiceCache_.erase(id);
    }
    LOG_INFO("Unloaded voice files for: " + character);
}

void VoiceManager::unloadAll() {
    for (auto& [id, entry] : voiceCache_) {
        if (entry.loaded) {
            UnloadSound(entry.sound);
        }
    }
    voiceCache_.clear();
    playing_ = false;
    currentVoiceId_.clear();
    LOG_INFO("Unloaded all voice files");
}

void VoiceManager::setBasePath(const std::string& path) {
    basePath_ = path;
    if (!basePath_.empty() && basePath_.back() != '/') {
        basePath_ += '/';
    }
}
