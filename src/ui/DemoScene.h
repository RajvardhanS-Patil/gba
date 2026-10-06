#pragma once
#include "core/SceneManager.h"
#include "dsa/ArrayDS.h"
#include "dsa/Stack.h"
#include "dsa/Queue.h"
#include "dsa/SortAlgorithms.h"
#include "dsa/ComplexityAnalyzer.h"
#include "ui/UISystem.h"
#include "save/SaveManager.h"
#include <vector>
#include <memory>

class DemoScene : public IScene {
public:
    DemoScene();
    ~DemoScene() override = default;

    void onEnter() override;
    void onExit() override;
    void update(float dt) override;
    void draw() override;
    std::string getName() const override { return "DemoScene"; }

private:
    enum class DemoStep {
        STORY_INTRO,
        CHARACTER_DIALOGUE,
        ARRAY_VISUALIZATION,
        STACK_GAMEPLAY,
        QUEUE_BUFFER,
        SORTING_VISUALIZATION,
        COMPLEXITY_ANALYSIS,
        OOP_ARCHITECTURE,
        SAVE_LOAD,
        FINAL_BOSS
    };

    DemoStep currentStep_ = DemoStep::STORY_INTRO;

    // Interactive DSA components for demo
    ArrayDS demoArray_;
    Stack demoStack_;
    Queue demoQueue_;
    ComplexityAnalyzer demoAnalyzer_;
    SaveManager saveManager_;

    // Sorting demo state
    std::vector<int> sortData_ = {64, 34, 25, 12, 22, 11, 90};
    std::vector<SortStep> sortSteps_;
    size_t currentSortStep_ = 0;
    float sortStepTimer_ = 0.0f;

    // UI Buttons for step navigation
    std::unique_ptr<UISystem::Button> nextBtn_;
    std::unique_ptr<UISystem::Button> prevBtn_;
    std::unique_ptr<UISystem::Button> backMenuBtn_;
    std::unique_ptr<UISystem::Button> actionBtn_;

    std::string demoStatusMessage_ = "";
};
