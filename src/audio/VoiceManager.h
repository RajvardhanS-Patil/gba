#pragma once
// ═══════════════════════════════════════════════════════════════════════════
// RUNTIME: The Last Algorithm
// VoiceManager — Character voice line playback
// ═══════════════════════════════════════════════════════════════════════════

#include "raylib.h"
#include "audio/AudioManager.h"
#include <string>
#include <unordered_map>
#include <functional>

class VoiceManager {
public:
    static VoiceManager& instance();

    VoiceManager(const VoiceManager&) = delete;
    VoiceManager& operator=(const VoiceManager&) = delete;

    // Play a voice line by ID (e.g., "nova_memory_001")
    // Looks up file in voices/ directory structure
    void play(const std::string& voiceId);

    // Stop current voice line
    void stop();

    // Skip to end of current voice line
    void skip();

    // Check state
    bool isPlaying() const;
    std::string getCurrentVoiceId() const;

    // Update — check if voice finished
    void update();

    // Set callback for when voice line finishes
    void setOnComplete(std::function<void()> callback);

    // Preload voice files for a character
    void preloadCharacter(const std::string& character);

    // Unload a character's voice files
    void unloadCharacter(const std::string& character);

    // Unload all
    void unloadAll();

    // Set base path for voice files (default: "assets/voices/")
    void setBasePath(const std::string& path);

private:
    VoiceManager();
    ~VoiceManager();

    std::string resolveVoicePath(const std::string& voiceId) const;

    struct VoiceEntry {
        Sound sound{};
        bool loaded = false;
    };

    std::unordered_map<std::string, VoiceEntry> voiceCache_;
    std::string currentVoiceId_;
    std::string basePath_ = "assets/voices/";
    bool playing_ = false;
    std::function<void()> onComplete_;
};
