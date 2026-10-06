#pragma once
#include <vector>
#include <memory>
#include "UISystem.h"
#include "core/SceneManager.h"

class PauseMenu : public IScene {
private:
    std::vector<std::unique_ptr<UISystem::Button>> buttons_;
    float anim_ = 0.0f;

public:
    PauseMenu();
    void onEnter() override;
    void onExit() override;
    void update(float dt) override;
    void draw() override;
    std::string getName() const override { return "PauseMenu"; }
};
