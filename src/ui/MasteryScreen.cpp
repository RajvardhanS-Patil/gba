#include "MasteryScreen.h"
#include "MainMenu.h"
#include "core/Game.h"
#include "audio/SFXManager.h"
#include "raylib.h"

extern const Color CYBER_CYAN;
extern const Color CYBER_DARK;
extern const Color CYBER_DIM;
extern const Color CYBER_PANEL;
extern const Color CYBER_TEXT;
extern const Color CYBER_SUCCESS;
extern const Color CYBER_WARNING;

MasteryScreen::MasteryScreen() {
    concepts_ = {
        {"ARRAY (O(1) ACCESS)", 1.0f, true},
        {"LINEAR SEARCH (O(N) SCAN)", 1.0f, true},
        {"BINARY SEARCH (O(LOG N))", 0.75f, true},
        {"LINKED LIST (O(1) INSERT)", 0.5f, true},
        {"STACK (LIFO)", 0.4f, true},
        {"QUEUE (FIFO BUFFER)", 0.4f, true},
        {"SORTING ALGORITHMS", 0.2f, true},
        {"BINARY SEARCH TREE", 0.0f, false},
        {"GRAPH TRAVERSAL (BFS/DFS)", 0.0f, false},
        {"DYNAMIC PROGRAMMING", 0.0f, false}
    };

    backBtn_ = std::make_unique<UISystem::Button>(Rectangle{50, 640, 180, 45}, "RETURN [ESC]", []() {
        Game::getSceneManager().changeScene(std::make_unique<MainMenu>());
    });
}

void MasteryScreen::onEnter() { enterAnim_ = 0.0f; }
void MasteryScreen::onExit() {}

void MasteryScreen::update(float dt) {
    if (enterAnim_ < 1.0f) enterAnim_ += dt * 3.0f;
    if (enterAnim_ > 1.0f) enterAnim_ = 1.0f;

    if (IsKeyPressed(KEY_ESCAPE)) {
        SFXManager::instance().play("click");
        Game::getSceneManager().changeScene(std::make_unique<MainMenu>());
        return;
    }

    backBtn_->update();
}

void MasteryScreen::draw() {
    ClearBackground(CYBER_DARK);

    // Grid backdrop
    for (int i = 0; i < 1280; i += 40) DrawLine(i, 0, i, 720, Color{255, 255, 255, 8});
    for (int i = 0; i < 720; i += 40) DrawLine(0, i, 1280, i, Color{255, 255, 255, 8});

    DrawText("OPERATOR DSA MASTERY MATRIX", 50, 40, 32, CYBER_CYAN);
    DrawText("Core algorithmic proficiencies recovered from corrupted sectors", 50, 78, 14, CYBER_DIM);
    
    for (size_t i = 0; i < concepts_.size(); i++) {
        float cx = 50.0f + (i % 2) * 590.0f;
        float cy = 115.0f + (i / 2) * 98.0f;
        
        Color baseCol = concepts_[i].unlocked ? CYBER_PANEL : Color{16, 20, 28, 255};
        Color textCol = concepts_[i].unlocked ? CYBER_CYAN : CYBER_DIM;
        Color borderCol = concepts_[i].unlocked ? Color{45, 65, 95, 255} : Color{25, 30, 42, 255};
        
        DrawRectangleRounded({cx, cy, 550, 82}, 0.12f, 6, baseCol);
        DrawRectangleRoundedLinesEx({cx, cy, 550, 82}, 0.12f, 6, 1.5f, borderCol);
        
        DrawText(concepts_[i].name.c_str(), static_cast<int>(cx + 20), static_cast<int>(cy + 18), 16, textCol);
        
        if (concepts_[i].unlocked) {
            float p = concepts_[i].progress * enterAnim_;
            DrawRectangleRounded({cx + 20, cy + 46, 420, 14}, 0.4f, 4, Color{10, 14, 22, 255});
            DrawRectangleRounded({cx + 20, cy + 46, 420 * p, 14}, 0.4f, 4, (p >= 1.0f) ? CYBER_SUCCESS : CYBER_CYAN);
            
            std::string pctStr = TextFormat("%d%%", (int)(p * 100));
            DrawText(pctStr.c_str(), static_cast<int>(cx + 455), static_cast<int>(cy + 45), 14, CYBER_TEXT);
        } else {
            DrawText("SECTOR LOCKED", static_cast<int>(cx + 380), static_cast<int>(cy + 30), 14, CYBER_DIM);
        }
    }
    
    backBtn_->draw();
}
