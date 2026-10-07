#include "GBAMainMenu.h"
#include "core/GBAEngine.h"
#include "core/Game.h"
#include "gameplay/GBAOverworldScene.h"
#include "audio/SFXManager.h"
#include <cmath>

GBAMainMenu::GBAMainMenu() {
    options_ = {"NEW GAME", "CONTINUE", "ALGORITHM INDEX", "OPTIONS"};
}

void GBAMainMenu::onEnter() {
    titleTimer_ = 0.0f;
    selectedIndex_ = 0;
}

void GBAMainMenu::onExit() {}

void GBAMainMenu::update(float dt) {
    titleTimer_ += dt;

    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        selectedIndex_ = (selectedIndex_ - 1 + static_cast<int>(options_.size())) % static_cast<int>(options_.size());
        SFXManager::instance().play("hover");
    } else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        selectedIndex_ = (selectedIndex_ + 1) % static_cast<int>(options_.size());
        SFXManager::instance().play("hover");
    }

    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_ENTER)) {
        SFXManager::instance().play("click");
        if (selectedIndex_ == 0 || selectedIndex_ == 1) {
            // Launch authentic GBA RPG Overworld
            Game::getSceneManager().changeScene(std::make_unique<GBAOverworldScene>("nova_town", 8, 8));
        }
    }
}

void GBAMainMenu::draw() {
    ClearBackground(GBA_DARK_BLUE);

    // Subtle 16px background pattern
    for (int y = 0; y < 160; y += 16) {
        for (int x = 0; x < 240; x += 16) {
            if ((x/16 + y/16) % 2 == 0) {
                DrawRectangle(x, y, 16, 16, Color{28, 42, 64, 255});
            }
        }
    }

    // Title banner box
    float bounce = sinf(titleTimer_ * 2.5f) * 2.0f;
    Rectangle titleCard = {24.0f, 18.0f + bounce, 192.0f, 44.0f};
    DrawRectangleRounded(titleCard, 0.15f, 4, GBA_CREAM);
    DrawRectangleRoundedLinesEx(titleCard, 0.15f, 4, 1.5f, GBA_GOLD);

    DrawText("RUNTIME", 64, static_cast<int>(24 + bounce), 22, GBA_DARK_BLUE);
    DrawText("THE LAST ALGORITHM", 62, static_cast<int>(46 + bounce), 8, GBA_CRIMSON);

    // Handheld RPG Menu Card
    Rectangle menuCard = {60.0f, 74.0f, 120.0f, 72.0f};
    DrawRectangleRounded(menuCard, 0.12f, 4, GBA_CREAM);
    DrawRectangleRoundedLinesEx(menuCard, 0.12f, 4, 1.5f, GBA_DARK_BLUE);

    for (size_t i = 0; i < options_.size(); ++i) {
        int oy = 82 + static_cast<int>(i) * 15;
        DrawText(options_[i].c_str(), 80, oy, 9, GBA_BLACK);

        if (static_cast<int>(i) == selectedIndex_) {
            // Triangle cursor
            DrawTriangle(
                Vector2{70.0f, static_cast<float>(oy + 1)},
                Vector2{70.0f, static_cast<float>(oy + 7)},
                Vector2{75.0f, static_cast<float>(oy + 4)},
                GBA_CRIMSON
            );
        }
    }

    // Subtitle Footer
    DrawText("2003 SYSTEM RUNNER CARTRIDGE", 50, 150, 7, GBA_LIGHT_BLUE);
}
