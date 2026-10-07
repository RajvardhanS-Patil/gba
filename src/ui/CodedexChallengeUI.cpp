#include "CodedexChallengeUI.h"
#include "core/GBAEngine.h"
#include "audio/SFXManager.h"
#include <cmath>

CodedexChallengeUI::CodedexChallengeUI() = default;

void CodedexChallengeUI::startChallenge(
    const std::string& questTitle,
    const std::string& promptText,
    const std::vector<std::string>& codeSnippetLines,
    const std::vector<ChallengeOption>& options,
    std::function<void(bool success)> onComplete
) {
    questTitle_ = questTitle;
    promptText_ = promptText;
    codeLines_ = codeSnippetLines;
    options_ = options;
    onComplete_ = onComplete;
    active_ = true;
    selectedIdx_ = 0;
    evaluated_ = false;
    answeredCorrect_ = false;
    feedbackTimer_ = 0.0f;
    feedbackMessage_ = "";

    SFXManager::instance().play("scan");
}

void CodedexChallengeUI::update(float dt) {
    if (!active_) return;

    if (evaluated_) {
        feedbackTimer_ += dt;
        if (feedbackTimer_ > 2.2f || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_ENTER)) {
            active_ = false;
            if (onComplete_) {
                onComplete_(answeredCorrect_);
            }
        }
        return;
    }

    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        selectedIdx_ = (selectedIdx_ - 1 + static_cast<int>(options_.size())) % static_cast<int>(options_.size());
        SFXManager::instance().play("hover");
    } else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        selectedIdx_ = (selectedIdx_ + 1) % static_cast<int>(options_.size());
        SFXManager::instance().play("hover");
    } else if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_X)) {
        active_ = false;
        if (onComplete_) onComplete_(false);
        SFXManager::instance().play("click");
        return;
    }

    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_ENTER)) {
        if (!options_.empty()) {
            evaluated_ = true;
            answeredCorrect_ = options_[selectedIdx_].isCorrect;
            feedbackTimer_ = 0.0f;
            if (answeredCorrect_) {
                SFXManager::instance().play("success");
            } else {
                SFXManager::instance().play("hit");
            }
        }
    }
}

void CodedexChallengeUI::draw() const {
    if (!active_) return;

    // Dim the overworld with semi-transparent tint
    DrawRectangle(0, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, Color{10, 14, 24, 210});

    // ─── CODÉDEX QUEST CHALLENGE WINDOW (224 x 146 px) ───
    Rectangle modalRec = {8.0f, 7.0f, 224.0f, 146.0f};
    DrawRectangleRounded(modalRec, 0.06f, 4, GBA_CREAM);
    DrawRectangleRoundedLinesEx(modalRec, 0.06f, 4, 1.5f, GBA_DARK_BLUE);

    // Top Header Banner
    DrawRectangle(10, 9, 220, 16, GBA_MID_BLUE);
    DrawText("CODEDEX // DSA QUEST TERMINAL", 16, 13, 8, GBA_WHITE);
    DrawText("20 XP", 195, 13, 8, GBA_GOLD);

    // Quest Title & Objective
    DrawText(questTitle_.c_str(), 14, 28, 9, GBA_BLACK);
    DrawText(promptText_.c_str(), 14, 40, 8, GBA_DARK_BLUE);

    // ─── CODE EDITOR / SNIPPET WINDOW (Terminal Style) ───
    Rectangle codeBox = {12.0f, 52.0f, 216.0f, 40.0f};
    DrawRectangleRounded(codeBox, 0.08f, 4, Color{24, 30, 42, 255});
    DrawRectangleRoundedLinesEx(codeBox, 0.08f, 4, 1.0f, GBA_MID_BLUE);

    // Code line numbering & syntax style
    for (size_t i = 0; i < codeLines_.size() && i < 3; ++i) {
        int cy = 56 + static_cast<int>(i) * 11;
        // Line number
        DrawText(TextFormat("%d", (int)(i + 1)), 16, cy, 7, Color{100, 120, 140, 255});
        DrawLine(24, 54, 24, 88, Color{45, 55, 75, 255});
        // Code text
        DrawText(codeLines_[i].c_str(), 28, cy, 8, GBA_WHITE);
    }

    // ─── INTERACTIVE MULTIPLE-CHOICE OPTIONS ───
    int optStartY = 96;
    for (size_t i = 0; i < options_.size() && i < 3; ++i) {
        int oy = optStartY + static_cast<int>(i) * 14;
        bool isSel = (static_cast<int>(i) == selectedIdx_);

        Rectangle optRec = {14.0f, static_cast<float>(oy), 212.0f, 12.0f};
        if (isSel) {
            DrawRectangleRounded(optRec, 0.2f, 2, GBA_WHITE);
            DrawRectangleRoundedLinesEx(optRec, 0.2f, 2, 1.0f, GBA_MID_BLUE);
            // Red arrow cursor
            DrawTriangle(
                Vector2{18.0f, static_cast<float>(oy + 2)},
                Vector2{18.0f, static_cast<float>(oy + 8)},
                Vector2{22.0f, static_cast<float>(oy + 5)},
                GBA_CRIMSON
            );
        }

        DrawText(options_[i].text.c_str(), 26, oy + 2, 8, isSel ? GBA_CRIMSON : GBA_BLACK);
    }

    // ─── EVALUATION FEEDBACK OVERLAY BANNER ───
    if (evaluated_ && selectedIdx_ >= 0 && selectedIdx_ < static_cast<int>(options_.size())) {
        Rectangle fbRec = {10.0f, 60.0f, 220.0f, 40.0f};
        Color fbBg = answeredCorrect_ ? GBA_GREEN_DARK : GBA_CRIMSON;
        DrawRectangleRounded(fbRec, 0.15f, 4, fbBg);
        DrawRectangleRoundedLinesEx(fbRec, 0.15f, 4, 1.5f, GBA_WHITE);

        const char* title = answeredCorrect_ ? "QUEST PASSED! (+20 XP)" : "LOGIC / COMPILATION BUG!";
        DrawText(title, 16, 64, 9, GBA_GOLD);
        DrawText(options_[selectedIdx_].explanation.c_str(), 16, 76, 7, GBA_WHITE);
        DrawText("[Z / SPACE] Continue", 140, 89, 7, GBA_CREAM);
    }
}
