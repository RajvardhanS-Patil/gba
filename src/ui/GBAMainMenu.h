#pragma once
#include "core/SceneManager.h"
#include <raylib.h>
#include <string>
#include <vector>

class GBAMainMenu : public IScene {
public:
    GBAMainMenu();
    ~GBAMainMenu() override = default;

    void onEnter() override;
    void onExit() override;
    void update(float dt) override;
    void draw() override;
    std::string getName() const override { return "GBAMainMenu"; }

private:
    int selectedIndex_ = 0;
    std::vector<std::string> options_;
    float titleTimer_ = 0.0f;
};
