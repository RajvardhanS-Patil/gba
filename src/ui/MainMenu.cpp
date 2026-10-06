#include "MainMenu.h"
#include "MasteryScreen.h"
#include "SettingsMenu.h"
#include "DemoScene.h"
#include "gameplay/MemoryDistrictScene.h"
#include "core/Game.h"
#include <cstdlib>

MainMenu::MainMenu() {
    float sw = 1280.0f;
    
    // NEW RUN
    auto newRunBtn = std::make_unique<UISystem::Button>(
        Rectangle{sw/2.0f - 110.0f, 280.0f, 220.0f, 42.0f}, "NEW RUN", []() {
            Game::getSceneManager().changeScene(std::make_unique<MemoryDistrictScene>());
        });
    buttons_.push_back(std::move(newRunBtn));

    // CONTINUE
    auto continueBtn = std::make_unique<UISystem::Button>(
        Rectangle{sw/2.0f - 110.0f, 335.0f, 220.0f, 42.0f}, "CONTINUE", []() {
            Game::getSceneManager().changeScene(std::make_unique<MemoryDistrictScene>());
        });
    buttons_.push_back(std::move(continueBtn));

    // DEMO MODE (For college viva presentation)
    auto demoBtn = std::make_unique<UISystem::Button>(
        Rectangle{sw/2.0f - 110.0f, 390.0f, 220.0f, 42.0f}, "DEMO MODE (VIVA)", []() {
            Game::getSceneManager().changeScene(std::make_unique<DemoScene>());
        });
    buttons_.push_back(std::move(demoBtn));

    // MASTERY SCREEN
    auto masteryBtn = std::make_unique<UISystem::Button>(
        Rectangle{sw/2.0f - 110.0f, 445.0f, 220.0f, 42.0f}, "DSA MASTERY", []() {
            Game::getSceneManager().changeScene(std::make_unique<MasteryScreen>());
        });
    buttons_.push_back(std::move(masteryBtn));

    // SETTINGS
    auto settingsBtn = std::make_unique<UISystem::Button>(
        Rectangle{sw/2.0f - 110.0f, 500.0f, 220.0f, 42.0f}, "SETTINGS", []() {
            Game::getSceneManager().changeScene(std::make_unique<SettingsMenu>());
        });
    buttons_.push_back(std::move(settingsBtn));

    // EXIT
    auto exitBtn = std::make_unique<UISystem::Button>(
        Rectangle{sw/2.0f - 110.0f, 555.0f, 220.0f, 42.0f}, "EXIT", []() {
            // Setting flag or closing window
        });
    buttons_.push_back(std::move(exitBtn));

    // Spawn ambient data particles
    for (int i = 0; i < 80; i++) {
        Particle p;
        p.pos = {static_cast<float>(rand() % 1280), static_cast<float>(rand() % 720)};
        p.vel = {0.0f, -10.0f - static_cast<float>(rand() % 20)};
        p.alpha = (rand() % 100) / 100.0f;
        p.size = (rand() % 3) + 1.0f;
        p.col = (rand() % 2 == 0) ? CYBER_CYAN : CYBER_VIOLET;
        particles_.push_back(p);
    }
}

void MainMenu::onEnter() { animTimer_ = 0.0f; }
void MainMenu::onExit() {}

void MainMenu::update(float dt) {
    animTimer_ += dt;
    for (auto& btn : buttons_) btn->update();

    for (auto& p : particles_) {
        p.pos.y += p.vel.y * dt;
        if (p.pos.y < -10.0f) p.pos.y = 730.0f;
    }
}

void MainMenu::draw() {
    ClearBackground(CYBER_DARK);

    // Subtle grid background
    for (int i = 0; i < 1280; i += 40) DrawLine(i, 0, i, 720, Color{255, 255, 255, 6});
    for (int i = 0; i < 720; i += 40) DrawLine(0, i, 1280, i, Color{255, 255, 255, 6});

    for (const auto& p : particles_) {
        Color c = p.col;
        c.a = static_cast<unsigned char>(p.alpha * 255);
        DrawCircleV(p.pos, p.size, c);
    }

    float tAlpha = (animTimer_ < 1.0f) ? animTimer_ : 1.0f;
    Color tCol = CYBER_CYAN;
    tCol.a = static_cast<unsigned char>(tAlpha * 255);
    
    const char* title = "RUNTIME";
    int tw = MeasureText(title, 80);
    DrawText(title, 1280/2 - tw/2, 85, 80, tCol);
    
    if (animTimer_ > 0.4f) {
        float subAlpha = (animTimer_ < 1.4f) ? (animTimer_ - 0.4f) : 1.0f;
        Color subCol = CYBER_DIM;
        subCol.a = static_cast<unsigned char>(subAlpha * 255);
        const char* sub = "THE LAST ALGORITHM";
        int sw = MeasureText(sub, 20);
        DrawText(sub, 1280/2 - sw/2, 175, 20, subCol);

        const char* tagline = "\"The world is dying. The only weapon left is intelligence.\"";
        int tgw = MeasureText(tagline, 14);
        Color tagCol = CYBER_TEXT;
        tagCol.a = static_cast<unsigned char>(subAlpha * 180);
        DrawText(tagline, 1280/2 - tgw/2, 205, 14, tagCol);
    }

    if (animTimer_ > 0.8f) {
        for (auto& btn : buttons_) btn->draw();
    }
    
    DrawText("v0.1.0 — Vertical Slice", 15, 720 - 25, 14, CYBER_DIM);
    DrawText("Controls: [WASD] Move  [E] Interact  [SPACE] Dialogue  [TAB] DSA Visualizer  [F1/V] Viva Inspector", 320, 720 - 25, 14, CYBER_DIM);
}
