// ═══════════════════════════════════════════════════════════════════════════
// RUNTIME: The Last Algorithm
// MusicManager — Implementation
// ═══════════════════════════════════════════════════════════════════════════

#include "audio/MusicManager.h"
#include "utils/Logger.h"
#include <filesystem>
#include <algorithm>

MusicManager& MusicManager::instance() {
    static MusicManager inst;
    return inst;
}

MusicManager::MusicManager() = default;

MusicManager::~MusicManager() {
    stop();
}

void MusicManager::play(const std::string& filepath, bool loop) {
    if (!AudioManager::instance().isInitialized()) {
        LOG_WARN("Cannot play music — audio not initialized");
        return;
    }

    // If same track is already playing, do nothing
    if (filepath == currentTrackPath_ && playing_) {
        return;
    }

    // Check if file exists
    if (!std::filesystem::exists(filepath)) {
        LOG_WARN("Music file not found: " + filepath);
        return;
    }

    // If something is currently playing, crossfade
    if (playing_ && musicLoaded_) {
        nextMusic_ = LoadMusicStream(filepath.c_str());
        nextMusic_.looping = loop;
        nextTrackPath_ = filepath;
        nextMusicLoaded_ = true;
        fadeState_ = FadeState::FADING_OUT;
        fadeProgress_ = 0.0f;
        LOG_INFO("Crossfading to: " + filepath);
    } else {
        // Nothing playing, just start
        if (musicLoaded_) {
            StopMusicStream(currentMusic_);
            UnloadMusicStream(currentMusic_);
        }

        currentMusic_ = LoadMusicStream(filepath.c_str());
        currentMusic_.looping = loop;
        currentTrackPath_ = filepath;
        musicLoaded_ = true;

        float vol = AudioManager::instance().getEffectiveMusic();
        SetMusicVolume(currentMusic_, vol);
        PlayMusicStream(currentMusic_);

        playing_ = true;
        fadeState_ = FadeState::FADING_IN;
        fadeProgress_ = 0.0f;
        LOG_INFO("Playing music: " + filepath);
    }
}

void MusicManager::stop() {
    if (musicLoaded_) {
        StopMusicStream(currentMusic_);
        UnloadMusicStream(currentMusic_);
        musicLoaded_ = false;
    }
    if (nextMusicLoaded_) {
        StopMusicStream(nextMusic_);
        UnloadMusicStream(nextMusic_);
        nextMusicLoaded_ = false;
    }
    playing_ = false;
    currentTrackPath_.clear();
    nextTrackPath_.clear();
    fadeState_ = FadeState::NONE;
}

void MusicManager::pause() {
    if (playing_ && musicLoaded_) {
        PauseMusicStream(currentMusic_);
    }
}

void MusicManager::resume() {
    if (musicLoaded_) {
        ResumeMusicStream(currentMusic_);
    }
}

void MusicManager::update(float dt) {
    if (!AudioManager::instance().isInitialized()) return;

    float musicVol = AudioManager::instance().getEffectiveMusic();

    // Update music stream
    if (musicLoaded_ && playing_) {
        UpdateMusicStream(currentMusic_);
    }
    if (nextMusicLoaded_) {
        UpdateMusicStream(nextMusic_);
    }

    // Handle crossfade
    switch (fadeState_) {
        case FadeState::FADING_OUT: {
            fadeProgress_ += dt / fadeDuration_;
            fadeProgress_ = std::min(fadeProgress_, 1.0f);

            if (musicLoaded_) {
                SetMusicVolume(currentMusic_, musicVol * (1.0f - fadeProgress_));
            }

            if (fadeProgress_ >= 1.0f) {
                // Old track faded out, switch to new
                if (musicLoaded_) {
                    StopMusicStream(currentMusic_);
                    UnloadMusicStream(currentMusic_);
                }

                currentMusic_ = nextMusic_;
                currentTrackPath_ = nextTrackPath_;
                musicLoaded_ = nextMusicLoaded_;
                nextMusicLoaded_ = false;
                nextTrackPath_.clear();

                if (musicLoaded_) {
                    SetMusicVolume(currentMusic_, 0.0f);
                    PlayMusicStream(currentMusic_);
                }

                fadeState_ = FadeState::FADING_IN;
                fadeProgress_ = 0.0f;
            }
            break;
        }
        case FadeState::FADING_IN: {
            fadeProgress_ += dt / fadeDuration_;
            fadeProgress_ = std::min(fadeProgress_, 1.0f);

            if (musicLoaded_) {
                SetMusicVolume(currentMusic_, musicVol * fadeProgress_);
            }

            if (fadeProgress_ >= 1.0f) {
                fadeState_ = FadeState::NONE;
            }
            break;
        }
        case FadeState::NONE: {
            // Keep volume synced with settings
            if (musicLoaded_ && playing_) {
                SetMusicVolume(currentMusic_, musicVol);
            }
            break;
        }
    }
}

void MusicManager::setCrossfadeDuration(float seconds) {
    fadeDuration_ = std::max(0.1f, seconds);
}

bool MusicManager::isPlaying() const {
    return playing_;
}

std::string MusicManager::getCurrentTrack() const {
    return currentTrackPath_;
}
