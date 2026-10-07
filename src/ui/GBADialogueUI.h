#pragma once
#include <raylib.h>
#include <string>

class GBADialogueUI {
public:
    GBADialogueUI();

    void start(const std::string& speaker, const std::string& line1, const std::string& line2 = "");
    void update(float dt);
    void draw() const;

    bool isActive() const { return active_; }
    bool isFinished() const { return finishedCurrent_; }
    void advance();
    void close();

private:
    bool active_ = false;
    std::string speaker_;
    std::string line1_;
    std::string line2_;

    size_t charIndex_ = 0;
    float typeTimer_ = 0.0f;
    float charDelay_ = 0.025f;
    bool finishedCurrent_ = false;

    Texture2D novaFace_{0};
    bool faceLoaded_ = false;
};
