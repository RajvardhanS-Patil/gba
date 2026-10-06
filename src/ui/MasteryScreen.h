#pragma once
#include <string>
#include <vector>
#include <memory>
#include "UISystem.h"

#include "core/SceneManager.h"

class MasteryScreen : public IScene {
private:
    struct Concept { std::string name; float progress; bool unlocked; };
    std::vector<Concept> concepts_;
    std::unique_ptr<UISystem::Button> backBtn_;
    float enterAnim_ = 0.0f;
    
public:
    MasteryScreen();
    void onEnter() override;
    void onExit() override;
    void update(float dt) override;
    void draw() override;
    std::string getName() const override { return "MasteryScreen"; }
};
