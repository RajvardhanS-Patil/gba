#include "DialogueUI.h"
#include "UISystem.h"
#include <cmath>

extern const Color CYBER_CYAN;
extern const Color CYBER_PANEL;
extern const Color CYBER_TEXT;
extern const Color CYBER_DIM;
extern const Color CYBER_DARK;

void DialogueUI::update(float dt, const std::string& text, bool& isComplete, const std::vector<std::string>& choices, int& selectedOut) {
    if (!isComplete) {
        typeTimer_ += dt;
        if (typeTimer_ > 0.05f) {
            typeTimer_ = 0.0f;
            if (static_cast<size_t>(charIndex_) < text.length()) charIndex_++;
            else isComplete = true;
        }
        if (IsKeyPressed(KEY_ENTER)) { charIndex_ = static_cast<int>(text.length()); isComplete = true; }
    } else if (!choices.empty()) {
        if (IsKeyPressed(KEY_UP)) selectedChoice_ = (selectedChoice_ - 1 + static_cast<int>(choices.size())) % static_cast<int>(choices.size());
        if (IsKeyPressed(KEY_DOWN)) selectedChoice_ = (selectedChoice_ + 1) % static_cast<int>(choices.size());
        if (IsKeyPressed(KEY_ENTER)) selectedOut = selectedChoice_;
    }
}

void DialogueUI::draw(const std::string& speaker, const std::string& text, bool& isComplete, const std::vector<std::string>& choices) {
    float pw = 1000;
    float ph = 180;
    float px = 1280/2 - pw/2;
    float py = 720 - ph - 20;

    UISystem::drawGlow({px, py, pw, ph}, CYBER_CYAN, 0.2f);
    DrawRectangleRounded({px, py, pw, ph}, 0.1f, 8, CYBER_PANEL);
    DrawRectangleRoundedLinesEx({px, py, pw, ph}, 0.1f, 8, 2.0f, CYBER_DIM);
    
    DrawRectangle(static_cast<int>(px + 20), static_cast<int>(py + 20), 80, 80, CYBER_DARK);
    DrawRectangleLines(static_cast<int>(px + 20), static_cast<int>(py + 20), 80, 80, CYBER_CYAN);
    
    DrawText(speaker.c_str(), static_cast<int>(px + 120), static_cast<int>(py + 20), 25, CYBER_CYAN);
    
    std::string displayTxt = text.substr(0, charIndex_);
    DrawText(displayTxt.c_str(), static_cast<int>(px + 120), static_cast<int>(py + 60), 20, CYBER_TEXT);

    if (isComplete && choices.empty()) {
        float pulse = (sin(GetTime() * 10) + 1.0f) * 0.5f;
        Color tc = CYBER_CYAN; tc.a = static_cast<unsigned char>(pulse * 255);
        DrawTriangle({px + pw - 30, py + ph - 20}, {px + pw - 40, py + ph - 40}, {px + pw - 20, py + ph - 40}, tc);
    }
    
    if (isComplete && !choices.empty()) {
        for (size_t i = 0; i < choices.size(); i++) {
            Color c = (static_cast<int>(i) == selectedChoice_) ? CYBER_CYAN : CYBER_DIM;
            DrawText((static_cast<int>(i) == selectedChoice_ ? "> " + choices[i] : choices[i]).c_str(), static_cast<int>(px + 120), static_cast<int>(py + 100 + i*25), 20, c);
        }
    }
}
