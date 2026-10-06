#include "Config.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "../utils/Logger.h"

using json = nlohmann::json;

Config& Config::getInstance() {
    static Config instance;
    return instance;
}

Config::Config() {}

void Config::load(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        LOG_WARN("Config file not found, using defaults: " + filepath);
        return;
    }
    
    try {
        json j;
        file >> j;
        if (j.contains("windowWidth")) windowWidth = j["windowWidth"];
        if (j.contains("windowHeight")) windowHeight = j["windowHeight"];
        if (j.contains("targetFPS")) targetFPS = j["targetFPS"];
        if (j.contains("fullscreen")) fullscreen = j["fullscreen"];
        if (j.contains("masterVolume")) masterVolume = j["masterVolume"];
        if (j.contains("musicVolume")) musicVolume = j["musicVolume"];
        if (j.contains("sfxVolume")) sfxVolume = j["sfxVolume"];
        if (j.contains("voiceVolume")) voiceVolume = j["voiceVolume"];
        if (j.contains("textSpeed")) textSpeed = j["textSpeed"];
        if (j.contains("vsync")) vsync = j["vsync"];
        LOG_INFO("Config loaded successfully from " + filepath);
    } catch (const std::exception& e) {
        LOG_ERROR("Failed to parse config: " + std::string(e.what()));
    }
}

void Config::save(const std::string& filepath) {
    json j;
    j["windowWidth"] = windowWidth;
    j["windowHeight"] = windowHeight;
    j["targetFPS"] = targetFPS;
    j["fullscreen"] = fullscreen;
    j["masterVolume"] = masterVolume;
    j["musicVolume"] = musicVolume;
    j["sfxVolume"] = sfxVolume;
    j["voiceVolume"] = voiceVolume;
    j["textSpeed"] = textSpeed;
    j["vsync"] = vsync;
    
    std::ofstream file(filepath);
    if (file.is_open()) {
        file << j.dump(4);
        LOG_INFO("Config saved successfully to " + filepath);
    } else {
        LOG_ERROR("Failed to save config to " + filepath);
    }
}

int Config::getWindowWidth() const { return windowWidth; }
void Config::setWindowWidth(int w) { windowWidth = w; }
int Config::getWindowHeight() const { return windowHeight; }
void Config::setWindowHeight(int h) { windowHeight = h; }
int Config::getTargetFPS() const { return targetFPS; }
void Config::setTargetFPS(int fps) { targetFPS = fps; }
bool Config::isFullscreen() const { return fullscreen; }
void Config::setFullscreen(bool f) { fullscreen = f; }
float Config::getMasterVolume() const { return masterVolume; }
void Config::setMasterVolume(float v) { masterVolume = v; }
float Config::getMusicVolume() const { return musicVolume; }
void Config::setMusicVolume(float v) { musicVolume = v; }
float Config::getSfxVolume() const { return sfxVolume; }
void Config::setSfxVolume(float v) { sfxVolume = v; }
float Config::getVoiceVolume() const { return voiceVolume; }
void Config::setVoiceVolume(float v) { voiceVolume = v; }
float Config::getTextSpeed() const { return textSpeed; }
void Config::setTextSpeed(float speed) { textSpeed = speed; }
bool Config::isVsync() const { return vsync; }
void Config::setVsync(bool v) { vsync = v; }
