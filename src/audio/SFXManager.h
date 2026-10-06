#pragma once
// ═══════════════════════════════════════════════════════════════════════════
// RUNTIME: The Last Algorithm
// SFXManager — Sound effects playback
// ═══════════════════════════════════════════════════════════════════════════

#include "raylib.h"
#include "audio/AudioManager.h"
#include <string>
#include <unordered_map>

class SFXManager {
public:
    static SFXManager& instance();

    SFXManager(const SFXManager&) = delete;
    SFXManager& operator=(const SFXManager&) = delete;

    // Preload a sound effect
    void preload(const std::string& id, const std::string& filepath);

    // Play a preloaded sound by ID
    void play(const std::string& id);

    // Play with custom pitch (1.0 = normal)
    void play(const std::string& id, float pitch);

    // Play a sound file directly (loads if needed)
    void playFile(const std::string& filepath);

    // Stop a specific sound
    void stop(const std::string& id);

    // Check if playing
    bool isPlaying(const std::string& id) const;

    // Unload all
    void unloadAll();

private:
    SFXManager();
    ~SFXManager();

    struct SFXEntry {
        Sound sound{};
        std::string filepath;
        bool loaded = false;
    };

    std::unordered_map<std::string, SFXEntry> sounds_;
};
