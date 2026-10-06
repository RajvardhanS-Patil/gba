#include "PauseMenu.h"
#include "raylib.h"

extern const Color CYBER_CYAN;
extern const Color CYBER_PANEL;

PauseMenu::PauseMenu() {
    float bx = 1280/2 - 100;
    buttons_.push_back(std::make_unique<UISystem::Button>((Rectangle){bx, 250, 200, 50}, "RESUME", [](){}));
    buttons_.push_back(std::make_unique<UISystem::Button>((Rectangle){bx, 320, 200, 50}, "SETTINGS", [](){}));
    buttons_.push_back(std::make_unique<UISystem::Button>((Rectangle){bx, 390, 200, 50}, "SAVE GAME", [](){}));
    buttons_.push_back(std::make_unique<UISystem::Button>((Rectangle){bx, 460, 200, 50}, "LOAD GAME", [](){}));
    buttons_.push_back(std::make_unique<UISystem::Button>((Rectangle){bx, 530, 200, 50}, "QUIT TO MENU", [](){}));
}

void PauseMenu::onEnter() { anim_ = 0.0f; }
void PauseMenu::onExit() {}

void PauseMenu::update(float dt) {
    if (anim_ < 1.0f) anim_ += dt * 5.0f;
    if (anim_ > 1.0f) anim_ = 1.0f;
    for (auto& btn : buttons_) btn->update();
}

void PauseMenu::draw() {
    DrawRectangle(0, 0, 1280, 720, Color{0, 0, 0, static_cast<unsigned char>(150 * anim_)});
    
    float pw = 300, ph = 450;
    float px = 1280/2.0f - pw/2.0f, py = 720/2.0f - ph/2.0f;
    
    DrawRectangleRounded({px, py, pw, ph}, 0.1f, 8, CYBER_PANEL);
    DrawRectangleRoundedLinesEx({px, py, pw, ph}, 0.1f, 8, 2.0f, CYBER_CYAN);
    
    DrawText("PAUSED", 1280/2 - MeasureText("PAUSED", 40)/2, static_cast<int>(py + 30), 40, CYBER_CYAN);
    
    for (auto& btn : buttons_) btn->draw();
}
