#include "HUD.h"
#include "UISystem.h"
#include <cmath>

extern const Color CYBER_CYAN;
extern const Color CYBER_DARK;
extern const Color CYBER_PANEL;
extern const Color CYBER_TEXT;
extern const Color CYBER_SUCCESS;
extern const Color CYBER_DANGER;
extern const Color CYBER_DIM;
extern const Color CYBER_WARNING;

void HUD::setHealth(int h, int m) { health_ = h; maxHealth_ = m; }
void HUD::setEnergy(int e, int m) { energy_ = e; maxEnergy_ = m; }
void HUD::setAbility(const std::string& name) { abilityName_ = name; }
void HUD::setObjective(const std::string& obj) { objective_ = obj; }
void HUD::setChapter(const std::string& chap) { chapter_ = chap; }
void HUD::showInteractionPrompt(bool show) { showPrompt_ = show; }

void HUD::update(float dt) {
    displayHealth_ += (health_ - displayHealth_) * dt * 6.0f;
    displayEnergy_ += (energy_ - displayEnergy_) * dt * 6.0f;
}

void HUD::draw() {
    float time = (float)GetTime();

    // ─── TOP-LEFT: STATUS GAUGES ───
    DrawRectangleRounded({16, 16, 260, 85}, 0.15f, 6, CYBER_PANEL);
    DrawRectangleRoundedLinesEx({16, 16, 260, 85}, 0.15f, 6, 1.5f, Color{40, 55, 80, 220});

    // Health Bar
    float hp = displayHealth_ / maxHealth_;
    Color hpCol = (hp > 0.3f) ? CYBER_SUCCESS : CYBER_DANGER;
    DrawText(TextFormat("INTEGRITY: %d / %d", health_, maxHealth_), 28, 24, 11, CYBER_TEXT);
    DrawRectangleRounded({28, 38, 236, 14}, 0.4f, 4, CYBER_DARK);
    DrawRectangleRounded({28, 38, 236 * hp, 14}, 0.4f, 4, hpCol);
    DrawRectangleRoundedLinesEx({28, 38, 236, 14}, 0.4f, 4, 1.0f, CYBER_DIM);

    // Energy Bar
    float ep = displayEnergy_ / maxEnergy_;
    DrawText(TextFormat("PROCESSOR EP: %d / %d", energy_, maxEnergy_), 28, 58, 11, CYBER_TEXT);
    DrawRectangleRounded({28, 72, 236, 12}, 0.4f, 4, CYBER_DARK);
    DrawRectangleRounded({28, 72, 236 * ep, 12}, 0.4f, 4, CYBER_CYAN);
    DrawRectangleRoundedLinesEx({28, 72, 236, 12}, 0.4f, 4, 1.0f, CYBER_DIM);

    // ─── EQUIPPED CORE BADGE ───
    DrawRectangleRounded({16, 110, 180, 36}, 0.2f, 6, CYBER_PANEL);
    DrawRectangleRoundedLinesEx({16, 110, 180, 36}, 0.2f, 6, 1.5f, CYBER_CYAN);
    DrawCircle(32, 128, 6.0f, CYBER_CYAN);
    DrawText(abilityName_.c_str(), 45, 121, 13, CYBER_CYAN);

    // ─── TOP-RIGHT: MISSION OBJECTIVE CARD ───
    int ow = MeasureText(objective_.c_str(), 14);
    float cardW = (float)ow + 50.0f;
    if (cardW < 280.0f) cardW = 280.0f;
    float cardX = 1280.0f - cardW - 16.0f;

    DrawRectangleRounded({cardX, 16, cardW, 58}, 0.15f, 6, CYBER_PANEL);
    DrawRectangleRoundedLinesEx({cardX, 16, cardW, 58}, 0.15f, 6, 1.5f, CYBER_WARNING);

    DrawText("PRIMARY DIRECTIVE", (int)cardX + 16, 24, 11, CYBER_WARNING);
    DrawText(objective_.c_str(), (int)cardX + 16, 42, 13, CYBER_TEXT);

    // ─── TOP-CENTER: CHAPTER TITLE BANNER ───
    int cw = MeasureText(chapter_.c_str(), 12);
    DrawRectangle(1280/2 - cw/2 - 16, 0, cw + 32, 24, Color{10, 14, 23, 220});
    DrawRectangleLines(1280/2 - cw/2 - 16, 0, cw + 32, 24, Color{0, 229, 255, 60});
    DrawText(chapter_.c_str(), 1280/2 - cw/2, 6, 12, CYBER_DIM);

    // ─── INTERACTION PROMPT ───
    if (showPrompt_) {
        float pulse = (sinf(time * 8.0f) + 1.0f) * 0.5f;
        int pw = MeasureText("[E] INTERACT WITH NODE", 16);
        float px = 1280/2 - pw/2 - 20;
        float py = 580;

        DrawRectangleRounded({px, py, (float)pw + 40, 38}, 0.3f, 6, CYBER_PANEL);
        DrawRectangleRoundedLinesEx({px, py, (float)pw + 40, 38}, 0.3f, 6, 2.0f, Color{0, 229, 255, (unsigned char)(150 + pulse * 105)});
        DrawText("[E] INTERACT WITH NODE", (int)px + 20, (int)py + 11, 16, CYBER_CYAN);
    }
}
