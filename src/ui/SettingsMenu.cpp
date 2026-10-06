#include "SettingsMenu.h"
#include "raylib.h"

extern const Color CYBER_CYAN;
extern const Color CYBER_DARK;

SettingsMenu::SettingsMenu() {
    masterVol_ = std::make_unique<UISystem::Slider>();
    masterVol_->bounds = {500, 200, 300, 20}; masterVol_->label = "Master Volume"; masterVol_->value = 1.0f;
    
    musicVol_ = std::make_unique<UISystem::Slider>();
    musicVol_->bounds = {500, 270, 300, 20}; musicVol_->label = "Music Volume"; musicVol_->value = 0.8f;
    
    sfxVol_ = std::make_unique<UISystem::Slider>();
    sfxVol_->bounds = {500, 340, 300, 20}; sfxVol_->label = "SFX Volume"; sfxVol_->value = 1.0f;
    
    textSpeed_ = std::make_unique<UISystem::Slider>();
    textSpeed_->bounds = {500, 410, 300, 20}; textSpeed_->label = "Text Speed"; textSpeed_->value = 0.5f;

    fsToggle_ = std::make_unique<UISystem::Button>((Rectangle){500, 480, 150, 40}, "FULLSCREEN: OFF", [this](){
        isFs_ = !isFs_;
        fsToggle_ = std::make_unique<UISystem::Button>((Rectangle){500, 480, 150, 40}, isFs_ ? "FULLSCREEN: ON" : "FULLSCREEN: OFF", [](){});
    });
    
    vsyncToggle_ = std::make_unique<UISystem::Button>((Rectangle){670, 480, 150, 40}, "VSYNC: ON", [this](){
        isVsync_ = !isVsync_;
        vsyncToggle_ = std::make_unique<UISystem::Button>((Rectangle){670, 480, 150, 40}, isVsync_ ? "VSYNC: ON" : "VSYNC: OFF", [](){});
    });

    applyBtn_ = std::make_unique<UISystem::Button>((Rectangle){500, 600, 140, 50}, "APPLY", [](){});
    backBtn_ = std::make_unique<UISystem::Button>((Rectangle){660, 600, 140, 50}, "BACK", [](){});
}

void SettingsMenu::onEnter() {}
void SettingsMenu::onExit() {}

void SettingsMenu::update(float dt) {
    masterVol_->update();
    musicVol_->update();
    sfxVol_->update();
    textSpeed_->update();
    fsToggle_->update();
    vsyncToggle_->update();
    applyBtn_->update();
    backBtn_->update();
}

void SettingsMenu::draw() {
    ClearBackground(CYBER_DARK);
    DrawText("SETTINGS", 1280/2 - MeasureText("SETTINGS", 50)/2, 50, 50, CYBER_CYAN);
    
    masterVol_->draw();
    musicVol_->draw();
    sfxVol_->draw();
    textSpeed_->draw();
    
    fsToggle_->draw();
    vsyncToggle_->draw();
    
    applyBtn_->draw();
    backBtn_->draw();
}
