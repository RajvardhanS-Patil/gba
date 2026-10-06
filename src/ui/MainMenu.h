#pragma once
#include <vector>
#include <string>
#include <memory>
#include "UISystem.h"
#include "raylib.h"

#include "core/SceneManager.h"

class MainMenu : public IScene {
private:
    struct Particle {
        Vector2 pos;
        Vector2 vel;
        float alpha;
        float size;
        Color col;
    };
    std::vector<Particle> particles_;
    std::vector<std::unique_ptr<UISystem::Button>> buttons_;
    float animTimer_ = 0.0f;

public:
    MainMenu();
    void onEnter() override;
    void onExit() override;
    void update(float dt) override;
    void draw() override;
    std::string getName() const override { return "MainMenu"; }
};
