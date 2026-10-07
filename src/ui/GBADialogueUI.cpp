#include "GBADialogueUI.h"
#include "core/GBAEngine.h"
#include "audio/SFXManager.h"
#include <cmath>

GBADialogueUI::GBADialogueUI() {
    if (FileExists("assets/sprites/nova.png")) {
        novaFace_ = LoadTexture("assets/sprites/nova.png");
        faceLoaded_ = true;
    }
}

void GBADialogueUI::start(const std::string& speaker, const std::string& line1, const std::string& line2) {
    speaker_ = speaker;
    line1_ = line1;
    line2_ = line2;
    active_ = true;
    charIndex_ = 0;
    typeTimer_ = 0.0f;
    finishedCurrent_ = false;
    SFXManager::instance().play("click");
}

void GBADialogueUI::update(float dt) {
    if (!active_) return;

    size_t totalLen = line1_.length() + (line2_.empty() ? 0 : line2_.length() + 1);

    if (!finishedCurrent_) {
        typeTimer_ += dt;
        if (typeTimer_ >= charDelay_) {
            typeTimer_ = 0.0f;
            if (charIndex_ < totalLen) {
                charIndex_++;
                if (charIndex_ % 3 == 0) {
                    SFXManager::instance().play("type", 1.2f);
                }
            } else {
                finishedCurrent_ = true;
            }
        }

        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_ENTER)) {
            charIndex_ = totalLen;
            finishedCurrent_ = true;
        }
    } else {
        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_ENTER)) {
            advance();
        }
    }
}

void GBADialogueUI::advance() {
    active_ = false;
    finishedCurrent_ = false;
    SFXManager::instance().play("click");
}

void GBADialogueUI::close() {
    active_ = false;
    finishedCurrent_ = false;
}

void GBADialogueUI::draw() const {
    if (!active_) return;

    // Handheld 240x160 classic bottom dialogue box:
    // x: 6, y: 110, w: 228, h: 46
    Rectangle boxRec = {6.0f, 110.0f, 228.0f, 46.0f};

    // Cream backdrop with 2px dark border
    DrawRectangleRounded(boxRec, 0.1f, 4, GBA_CREAM);
    DrawRectangleRoundedLinesEx(boxRec, 0.1f, 4, 1.5f, GBA_DARK_BLUE);

    // Speaker Name Banner if speaker present
    if (!speaker_.empty()) {
        int sw = MeasureText(speaker_.c_str(), 10);
        Rectangle nameBox = {10.0f, 100.0f, static_cast<float>(sw + 12), 13.0f};
        DrawRectangleRounded(nameBox, 0.2f, 4, GBA_MID_BLUE);
        DrawRectangleRoundedLinesEx(nameBox, 0.2f, 4, 1.0f, GBA_WHITE);
        DrawText(speaker_.c_str(), 16, 102, 10, GBA_WHITE);
    }

    // Split displayed text between Line 1 and Line 2
    std::string fullText = line1_ + (line2_.empty() ? "" : "\n" + line2_);
    std::string displayed = fullText.substr(0, charIndex_);

    size_t newlinePos = displayed.find('\n');
    std::string dispLine1 = (newlinePos != std::string::npos) ? displayed.substr(0, newlinePos) : displayed;
    std::string dispLine2 = (newlinePos != std::string::npos) ? displayed.substr(newlinePos + 1) : "";

    // Text draw inside box
    DrawText(dispLine1.c_str(), 14, 118, 10, GBA_BLACK);
    if (!dispLine2.empty()) {
        DrawText(dispLine2.c_str(), 14, 132, 10, GBA_BLACK);
    }

    // Bouncing GBA continue arrow [v]
    if (finishedCurrent_) {
        float bounce = sinf(static_cast<float>(GetTime()) * 8.0f) * 2.0f;
        int ax = 224;
        int ay = static_cast<int>(142 + bounce);
        DrawTriangle(
            Vector2{static_cast<float>(ax - 4), static_cast<float>(ay - 4)},
            Vector2{static_cast<float>(ax + 4), static_cast<float>(ay - 4)},
            Vector2{static_cast<float>(ax), static_cast<float>(ay)},
            GBA_CRIMSON
        );
    }
}
