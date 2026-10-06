#pragma once
// ═══════════════════════════════════════════════════════════════════════════
// RUNTIME: The Last Algorithm
// MusicManager — Background music playback with crossfade
// ═══════════════════════════════════════════════════════════════════════════

#include "raylib.h"
#include "audio/AudioManager.h"
#include <string>
#include <unordered_map>

class MusicManager {
public:
    static MusicManager& instance();

    MusicManager(const MusicManager&) = delete;
    MusicManager& operator=(const MusicManager&) = delete;

    // Play a music track (crossfades from current)
    void play(const std::string& filepath, bool loop = true);

    // Stop current music
    void stop();

    // Pause/Resume
    void pause();
    void resume();

    // Update — must be called every frame for streaming
    void update(float dt);

    // Crossfade settings
    void setCrossfadeDuration(float seconds);

    // State
    bool isPlaying() const;
    std::string getCurrentTrack() const;

private:
    MusicManager();
    ~MusicManager();

    enum class FadeState {
        NONE,
        FADING_OUT,
        FADING_IN
    };

    Music currentMusic_{};
    Music nextMusic_{};
    std::string currentTrackPath_;
    std::string nextTrackPath_;

    FadeState fadeState_ = FadeState::NONE;
    float fadeProgress_ = 0.0f;
    float fadeDuration_ = 1.5f;

    bool playing_ = false;
    bool musicLoaded_ = false;
    bool nextMusicLoaded_ = false;
};
