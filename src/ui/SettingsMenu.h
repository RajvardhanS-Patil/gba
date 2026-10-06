#pragma once
#include <memory>
#include <vector>
#include "UISystem.h"
#include "core/SceneManager.h"

class SettingsMenu : public IScene {
private:
    std::unique_ptr<UISystem::Slider> masterVol_;
    std::unique_ptr<UISystem::Slider> musicVol_;
    std::unique_ptr<UISystem::Slider> sfxVol_;
    std::unique_ptr<UISystem::Slider> textSpeed_;
    
    std::unique_ptr<UISystem::Button> fsToggle_;
    std::unique_ptr<UISystem::Button> vsyncToggle_;
    std::unique_ptr<UISystem::Button> applyBtn_;
    std::unique_ptr<UISystem::Button> backBtn_;

    bool isFs_ = false;
    bool isVsync_ = true;

public:
    SettingsMenu();
    void onEnter() override;
    void onExit() override;
    void update(float dt) override;
    void draw() override;
    std::string getName() const override { return "SettingsMenu"; }
};
