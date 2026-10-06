#include "HUD.h"
#include "UISystem.h"

extern const Color CYBER_CYAN;
extern const Color CYBER_DARK;
extern const Color CYBER_PANEL;
extern const Color CYBER_TEXT;
extern const Color CYBER_SUCCESS;
extern const Color CYBER_DANGER;

void HUD::setHealth(int h, int m) { health_ = h; maxHealth_ = m; }
void HUD::setEnergy(int e, int m) { energy_ = e; maxEnergy_ = m; }
void HUD::setAbility(const std::string& name) { abilityName_ = name; }
void HUD::setObjective(const std::string& obj) { objective_ = obj; }
void HUD::setChapter(const std::string& chap) { chapter_ = chap; }
void HUD::showInteractionPrompt(bool show) { showPrompt_ = show; }

void HUD::update(float dt) {
    displayHealth_ += (health_ - displayHealth_) * dt * 5.0f;
    displayEnergy_ += (energy_ - displayEnergy_) * dt * 5.0f;
}

void HUD::draw() {
    // Health
    DrawRectangleRounded({20, 20, 200, 20}, 0.5f, 4, CYBER_DARK);
    float hp = displayHealth_ / maxHealth_;
    Color hpCol = (hp > 0.3f) ? CYBER_SUCCESS : CYBER_DANGER;
    DrawRectangleRounded({20, 20, 200 * hp, 20}, 0.5f, 4, hpCol);
    DrawRectangleRoundedLinesEx({20, 20, 200, 20}, 0.5f, 4, 2.0f, CYBER_TEXT);
    DrawText(TextFormat("HP %d/%d", health_, maxHealth_), 25, 23, 10, CYBER_TEXT);

    // Energy
    DrawRectangleRounded({20, 50, 150, 15}, 0.5f, 4, CYBER_DARK);
    float ep = displayEnergy_ / maxEnergy_;
    DrawRectangleRounded({20, 50, 150 * ep, 15}, 0.5f, 4, CYBER_CYAN);
    DrawRectangleRoundedLinesEx({20, 50, 150, 15}, 0.5f, 4, 2.0f, CYBER_TEXT);
    DrawText(TextFormat("EP %d/%d", energy_, maxEnergy_), 25, 52, 10, CYBER_TEXT);

    // Ability
    DrawRectangleRounded({20, 75, 40, 40}, 0.2f, 4, CYBER_PANEL);
    DrawRectangleRoundedLinesEx({20, 75, 40, 40}, 0.2f, 4, 2.0f, CYBER_CYAN);
    DrawText(abilityName_.c_str(), 70, 85, 20, CYBER_CYAN);

    // Objective
    DrawText(objective_.c_str(), 1280 - MeasureText(objective_.c_str(), 20) - 20, 20, 20, CYBER_TEXT);

    // Chapter
    DrawText(chapter_.c_str(), 1280/2 - MeasureText(chapter_.c_str(), 15)/2, 10, 15, {100, 110, 140, 150});

    // Prompt
    if (showPrompt_) {
        DrawText("[E] INTERACT", 1280/2 - MeasureText("[E] INTERACT", 20)/2, 600, 20, CYBER_CYAN);
    }
}
