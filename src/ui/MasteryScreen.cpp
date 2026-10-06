#include "MasteryScreen.h"
#include "raylib.h"

extern const Color CYBER_CYAN;
extern const Color CYBER_DARK;
extern const Color CYBER_DIM;
extern const Color CYBER_PANEL;

MasteryScreen::MasteryScreen() {
    concepts_ = {
        {"ARRAY", 1.0f, true},
        {"LINKED LIST", 0.5f, true},
        {"STACK", 0.0f, true},
        {"QUEUE", 0.0f, true},
        {"SORTING", 0.0f, false},
        {"TREE", 0.0f, false},
        {"GRAPH", 0.0f, false}
    };
    backBtn_ = std::make_unique<UISystem::Button>((Rectangle){50, 630, 150, 40}, "BACK", [](){});
}

void MasteryScreen::onEnter() { enterAnim_ = 0.0f; }
void MasteryScreen::onExit() {}

void MasteryScreen::update(float dt) {
    if (enterAnim_ < 1.0f) enterAnim_ += dt;
    if (enterAnim_ > 1.0f) enterAnim_ = 1.0f;
    backBtn_->update();
}

void MasteryScreen::draw() {
    ClearBackground(CYBER_DARK);
    DrawText("DSA MASTERY", 50, 50, 40, CYBER_CYAN);
    
    for (size_t i = 0; i < concepts_.size(); i++) {
        float cx = 50.0f + (i % 2) * 500.0f;
        float cy = 150.0f + (i / 2) * 100.0f;
        
        Color baseCol = concepts_[i].unlocked ? CYBER_PANEL : Color{20, 20, 20, 255};
        Color textCol = concepts_[i].unlocked ? CYBER_CYAN : CYBER_DIM;
        
        DrawRectangleRounded({cx, cy, 400, 80}, 0.1f, 8, baseCol);
        DrawRectangleRoundedLinesEx({cx, cy, 400, 80}, 0.1f, 8, 2.0f, textCol);
        
        DrawText(concepts_[i].name.c_str(), static_cast<int>(cx + 20), static_cast<int>(cy + 20), 20, textCol);
        
        if (concepts_[i].unlocked) {
            float p = concepts_[i].progress * enterAnim_;
            DrawRectangle(static_cast<int>(cx + 20), static_cast<int>(cy + 50), 360, 10, CYBER_DIM);
            DrawRectangle(static_cast<int>(cx + 20), static_cast<int>(cy + 50), static_cast<int>(360 * p), 10, CYBER_CYAN);
        } else {
            DrawText("LOCKED", static_cast<int>(cx + 320), static_cast<int>(cy + 30), 15, CYBER_DIM);
        }
    }
    
    backBtn_->draw();
}
