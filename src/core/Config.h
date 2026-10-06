#pragma once
#include <string>

class Config {
public:
    static Config& getInstance();
    
    void load(const std::string& filepath);
    void save(const std::string& filepath);
    
    int getWindowWidth() const;
    void setWindowWidth(int w);
    
    int getWindowHeight() const;
    void setWindowHeight(int h);
    
    int getTargetFPS() const;
    void setTargetFPS(int fps);
    
    bool isFullscreen() const;
    void setFullscreen(bool f);
    
    float getMasterVolume() const;
    void setMasterVolume(float v);
    
    float getMusicVolume() const;
    void setMusicVolume(float v);
    
    float getSfxVolume() const;
    void setSfxVolume(float v);
    
    float getVoiceVolume() const;
    void setVoiceVolume(float v);
    
    float getTextSpeed() const;
    void setTextSpeed(float speed);
    
    bool isVsync() const;
    void setVsync(bool v);

private:
    Config();
    ~Config() = default;
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    int windowWidth{1280};
    int windowHeight{720};
    int targetFPS{60};
    bool fullscreen{false};
    float masterVolume{0.8f};
    float musicVolume{0.7f};
    float sfxVolume{0.8f};
    float voiceVolume{0.9f};
    float textSpeed{30.0f};
    bool vsync{true};
};
