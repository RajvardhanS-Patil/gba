#include "SettingsMenu.h"
#include "MainMenu.h"
#include "core/Game.h"
#include "audio/AudioManager.h"
#include "audio/SFXManager.h"
#include "raylib.h"

extern const Color CYBER_CYAN;
extern const Color CYBER_DARK;
extern const Color CYBER_PANEL;
extern const Color CYBER_TEXT;
extern const Color CYBER_DIM;
extern const Color CYBER_SUCCESS;

SettingsMenu::SettingsMenu() {
    float cx = 1280/2.0f - 200.0f;

    masterVol_ = std::make_unique<UISystem::Slider>();
    masterVol_->bounds = {cx, 160, 400, 18}; masterVol_->label = "Master Volume"; masterVol_->value = AudioManager::instance().getMasterVolume();
    
    musicVol_ = std::make_unique<UISystem::Slider>();
    musicVol_->bounds = {cx, 230, 400, 18}; musicVol_->label = "Music Volume"; musicVol_->value = AudioManager::instance().getMusicVolume();
    
    sfxVol_ = std::make_unique<UISystem::Slider>();
    sfxVol_->bounds = {cx, 300, 400, 18}; sfxVol_->label = "SFX Volume"; sfxVol_->value = AudioManager::instance().getSFXVolume();
    
    textSpeed_ = std::make_unique<UISystem::Slider>();
    textSpeed_->bounds = {cx, 370, 400, 18}; textSpeed_->label = "Terminal Baud Rate / Text Speed"; textSpeed_->value = 0.8f;

    fsToggle_ = std::make_unique<UISystem::Button>(Rectangle{cx, 440, 190, 40}, "FULLSCREEN: OFF", [this]() {
        isFs_ = !isFs_;
        ToggleFullscreen();
    });
    
    vsyncToggle_ = std::make_unique<UISystem::Button>(Rectangle{cx + 210, 440, 190, 40}, "VSYNC: ENABLED", [this]() {
        isVsync_ = !isVsync_;
    });

    applyBtn_ = std::make_unique<UISystem::Button>(Rectangle{cx, 540, 190, 45}, "APPLY CHANGES", [this]() {
        AudioManager::instance().setMasterVolume(masterVol_->getValue());
        AudioManager::instance().setMusicVolume(musicVol_->getValue());
        AudioManager::instance().setSFXVolume(sfxVol_->getValue());
        SFXManager::instance().play("success");
    });

    backBtn_ = std::make_unique<UISystem::Button>(Rectangle{cx + 210, 540, 190, 45}, "RETURN [ESC]", []() {
        Game::getSceneManager().changeScene(std::make_unique<MainMenu>());
    });
}

void SettingsMenu::onEnter() {}
void SettingsMenu::onExit() {}

void SettingsMenu::update(float dt) {
    masterVol_->update();
    musicVol_->update();
    sfxVol_->update();
    textSpeed_->update();

    if (IsKeyPressed(KEY_ESCAPE)) {
        SFXManager::instance().play("click");
        Game::getSceneManager().changeScene(std::make_unique<MainMenu>());
        return;
    }

    fsToggle_->update();
    vsyncToggle_->update();
    applyBtn_->update();
    backBtn_->update();
}

void SettingsMenu::draw() {
    ClearBackground(CYBER_DARK);

    // Subtle grid
    for (int i = 0; i < 1280; i += 40) DrawLine(i, 0, i, 720, Color{255, 255, 255, 8});
    for (int i = 0; i < 720; i += 40) DrawLine(0, i, 1280, i, Color{255, 255, 255, 8});

    DrawText("SYSTEM CONFIGURATION", 1280/2 - MeasureText("SYSTEM CONFIGURATION", 36)/2, 55, 36, CYBER_CYAN);
    
    // Panel container
    float cx = 1280/2.0f - 240.0f;
    DrawRectangleRounded({cx, 120, 480, 500}, 0.08f, 8, CYBER_PANEL);
    DrawRectangleRoundedLinesEx({cx, 120, 480, 500}, 0.08f, 8, 1.5f, Color{45, 60, 85, 255});

    masterVol_->draw();
    musicVol_->draw();
    sfxVol_->draw();
    textSpeed_->draw();
    
    fsToggle_->draw();
    vsyncToggle_->draw();
    
    applyBtn_->draw();
    backBtn_->draw();
}
