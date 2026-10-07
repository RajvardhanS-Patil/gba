#include "DialogueUI.h"
#include "UISystem.h"
#include "audio/SFXManager.h"
#include <cmath>

extern const Color CYBER_CYAN;
extern const Color CYBER_PANEL;
extern const Color CYBER_TEXT;
extern const Color CYBER_DIM;
extern const Color CYBER_DARK;
extern const Color CYBER_VIOLET;
extern const Color CYBER_WARNING;

void DialogueUI::update(float dt, const std::string& text, bool& isComplete, const std::vector<std::string>& choices, int& selectedOut) {
    if (!isComplete) {
        typeTimer_ += dt;
        if (typeTimer_ > 0.035f) {
            typeTimer_ = 0.0f;
            if (static_cast<size_t>(charIndex_) < text.length()) {
                charIndex_++;
                if (charIndex_ % 3 == 0) {
                    SFXManager::instance().play("type", 1.0f + ((charIndex_ % 5) * 0.05f));
                }
            } else {
                isComplete = true;
            }
        }
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) { 
            charIndex_ = static_cast<int>(text.length()); 
            isComplete = true; 
        }
    } else if (!choices.empty()) {
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
            selectedChoice_ = (selectedChoice_ - 1 + static_cast<int>(choices.size())) % static_cast<int>(choices.size());
            SFXManager::instance().play("hover");
        }
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
            selectedChoice_ = (selectedChoice_ + 1) % static_cast<int>(choices.size());
            SFXManager::instance().play("hover");
        }
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            SFXManager::instance().play("click");
            selectedOut = selectedChoice_;
        }
    }
}

void DialogueUI::draw(const std::string& speaker, const std::string& text, bool& isComplete, const std::vector<std::string>& choices) {
    float pw = 1040;
    float ph = 190;
    float px = 1280/2 - pw/2;
    float py = 720 - ph - 24;

    // Glowing frame
    UISystem::drawGlow({px, py, pw, ph}, (speaker == "kael") ? CYBER_CYAN : CYBER_VIOLET, 0.25f);
    DrawRectangleRounded({px, py, pw, ph}, 0.08f, 8, Color{16, 22, 34, 250});
    DrawRectangleRoundedLinesEx({px, py, pw, ph}, 0.08f, 8, 2.0f, (speaker == "kael") ? CYBER_CYAN : CYBER_VIOLET);
    
    // Speaker Avatar Portrait Frame
    DrawRectangleRounded({px + 20, py + 20, 90, 90}, 0.15f, 6, CYBER_DARK);
    DrawRectangleRoundedLinesEx({px + 20, py + 20, 90, 90}, 0.15f, 6, 2.0f, (speaker == "kael") ? CYBER_CYAN : CYBER_VIOLET);
    
    // Draw portrait avatar
    if (speaker == "kael") {
        DrawCircle((int)(px + 65), (int)(py + 55), 24, CYBER_PANEL);
        DrawRectangle((int)(px + 53), (int)(py + 51), 24, 8, CYBER_CYAN); // Visor
        DrawText("OP", (int)(px + 57), (int)(py + 75), 10, CYBER_CYAN);
    } else if (speaker == "nova") {
        DrawCircle((int)(px + 65), (int)(py + 65), 24, CYBER_VIOLET);
        DrawCircle((int)(px + 65), (int)(py + 65), 12, WHITE);
    } else {
        DrawText("SYS", (int)(px + 50), (int)(py + 55), 18, CYBER_WARNING);
    }

    // Speaker Name Tag
    std::string tag = (speaker == "kael") ? "KAEL [FIELD ENGINEER]" : (speaker == "nova" ? "NOVA [RUNTIME AI]" : "SYSTEM OS");
    DrawText(tag.c_str(), static_cast<int>(px + 130), static_cast<int>(py + 20), 20, (speaker == "kael") ? CYBER_CYAN : CYBER_VIOLET);
    
    // Spoken Text
    std::string displayTxt = text.substr(0, charIndex_);
    DrawText(displayTxt.c_str(), static_cast<int>(px + 130), static_cast<int>(py + 55), 18, CYBER_TEXT);

    // Indicator when ready to continue
    if (isComplete && choices.empty()) {
        float pulse = (sinf((float)GetTime() * 8.0f) + 1.0f) * 0.5f;
        Color tc = CYBER_CYAN; 
        tc.a = static_cast<unsigned char>(120 + pulse * 135);
        DrawText("[SPACE / ENTER] ADVANCE", static_cast<int>(px + pw - 220), static_cast<int>(py + ph - 28), 13, tc);
    }
    
    // Choices list
    if (isComplete && !choices.empty()) {
        for (size_t i = 0; i < choices.size(); i++) {
            Color c = (static_cast<int>(i) == selectedChoice_) ? CYBER_CYAN : CYBER_DIM;
            DrawText((static_cast<int>(i) == selectedChoice_ ? "> " + choices[i] : "  " + choices[i]).c_str(), 
                     static_cast<int>(px + 130), static_cast<int>(py + 100 + i*24), 18, c);
        }
    }
}
