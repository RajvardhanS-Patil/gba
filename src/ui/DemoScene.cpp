#include "DemoScene.h"
#include "MainMenu.h"
#include "core/Game.h"
#include "utils/Logger.h"
#include <string>

DemoScene::DemoScene() {
    // Fill demo array
    demoArray_.insert(10);
    demoArray_.insert(25);
    demoArray_.insert(42);
    demoArray_.insert(88);
    demoArray_.insert(99);

    // Fill demo stack
    demoStack_.push(10);
    demoStack_.push(20);
    demoStack_.push(30);

    // Fill demo queue
    demoQueue_.enqueue(100);
    demoQueue_.enqueue(200);
    demoQueue_.enqueue(300);

    // Compute sorting steps for demo
    sortSteps_ = BubbleSort::sort(sortData_);

    // Fill complexity records
    demoAnalyzer_.addRecord({"Array Access", ComplexityClass::O_1, 1000, 1, 0.001f});
    demoAnalyzer_.addRecord({"Binary Search", ComplexityClass::O_LOG_N, 1000, 10, 0.005f});
    demoAnalyzer_.addRecord({"Linear Search", ComplexityClass::O_N, 1000, 1000, 0.05f});
    demoAnalyzer_.addRecord({"Merge Sort", ComplexityClass::O_N_LOG_N, 1000, 9965, 0.12f});
    demoAnalyzer_.addRecord({"Bubble Sort", ComplexityClass::O_N_SQUARED, 1000, 1000000, 1.5f});

    // Create navigation buttons
    prevBtn_ = std::make_unique<UISystem::Button>(
        Rectangle{50, 650, 140, 45}, "PREV [P]", [this]() {
            int step = static_cast<int>(currentStep_);
            if (step > 0) currentStep_ = static_cast<DemoStep>(step - 1);
        });

    nextBtn_ = std::make_unique<UISystem::Button>(
        Rectangle{210, 650, 140, 45}, "NEXT [N]", [this]() {
            int step = static_cast<int>(currentStep_);
            if (step < 9) currentStep_ = static_cast<DemoStep>(step + 1);
        });

    actionBtn_ = std::make_unique<UISystem::Button>(
        Rectangle{370, 650, 180, 45}, "OPERATE [SPACE]", [this]() {
            if (currentStep_ == DemoStep::STACK_GAMEPLAY) {
                if (demoStack_.size() < 6) demoStack_.push(rand() % 90 + 10);
                else demoStack_.pop();
            } else if (currentStep_ == DemoStep::QUEUE_BUFFER) {
                if (demoQueue_.size() < 6) demoQueue_.enqueue(rand() % 90 + 10);
                else demoQueue_.dequeue();
            } else if (currentStep_ == DemoStep::ARRAY_VISUALIZATION) {
                demoArray_.search(42);
            } else if (currentStep_ == DemoStep::SAVE_LOAD) {
                SaveData d;
                d.chapter = 1;
                d.playerHealth = 100;
                d.unlockedAbilities.push_back("ARRAY_CORE");
                saveManager_.save(1, d);
                demoStatusMessage_ = "Saved slot 1 successfully!";
            }
        });

    backMenuBtn_ = std::make_unique<UISystem::Button>(
        Rectangle{1080, 650, 150, 45}, "MENU [ESC]", []() {
            Game::getSceneManager().changeScene(std::make_unique<MainMenu>());
        });
}

void DemoScene::onEnter() {
    LOG_INFO("Entered Viva / Presentation Demo Mode");
}

void DemoScene::onExit() {
    LOG_INFO("Exiting Demo Mode");
}

void DemoScene::update(float dt) {
    demoStack_.updateAnim(dt);
    demoQueue_.updateAnim(dt);

    // Keyboard shortcuts
    if (IsKeyPressed(KEY_N) || IsKeyPressed(KEY_RIGHT)) {
        int step = static_cast<int>(currentStep_);
        if (step < 9) currentStep_ = static_cast<DemoStep>(step + 1);
    }
    if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_LEFT)) {
        int step = static_cast<int>(currentStep_);
        if (step > 0) currentStep_ = static_cast<DemoStep>(step - 1);
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        Game::getSceneManager().changeScene(std::make_unique<MainMenu>());
        return;
    }

    // Step-by-step sorting playback
    if (currentStep_ == DemoStep::SORTING_VISUALIZATION) {
        sortStepTimer_ += dt;
        if (sortStepTimer_ > 0.4f && !sortSteps_.empty()) {
            sortStepTimer_ = 0.0f;
            currentSortStep_ = (currentSortStep_ + 1) % sortSteps_.size();
        }
    }

    prevBtn_->update();
    nextBtn_->update();
    actionBtn_->update();
    backMenuBtn_->update();
}

void DemoScene::draw() {
    ClearBackground(CYBER_DARK);

    // Top Header Banner
    DrawRectangle(0, 0, 1280, 70, CYBER_PANEL);
    DrawRectangleLinesEx(Rectangle{0, 0, 1280, 70}, 2.0f, CYBER_CYAN);
    DrawText("RUNTIME: COLLEGE VIVA & PRESENTATION DEMO MODE", 40, 20, 24, CYBER_CYAN);
    
    std::string stepTitle = "";
    switch (currentStep_) {
        case DemoStep::STORY_INTRO:            stepTitle = "1/10: NARRATIVE ARC & EIDOLON RUNTIME"; break;
        case DemoStep::CHARACTER_DIALOGUE:     stepTitle = "2/10: CHARACTER DIALOGUE & AUDIO HOOKS"; break;
        case DemoStep::ARRAY_VISUALIZATION:    stepTitle = "3/10: ARRAY BUFFER & LINEAR SEARCH (O(n))"; break;
        case DemoStep::STACK_GAMEPLAY:         stepTitle = "4/10: STACK CORE & LIFO REVERSAL (O(1))"; break;
        case DemoStep::QUEUE_BUFFER:           stepTitle = "5/10: QUEUE BUFFER & FIFO ORDERING (O(1))"; break;
        case DemoStep::SORTING_VISUALIZATION:  stepTitle = "6/10: BUBBLE SORT STEP-BY-STEP (O(n^2))"; break;
        case DemoStep::COMPLEXITY_ANALYSIS:    stepTitle = "7/10: BIG-O COMPLEXITY COMPARISON"; break;
        case DemoStep::OOP_ARCHITECTURE:       stepTitle = "8/10: C++ OOP ARCHITECTURE BLUEPRINT"; break;
        case DemoStep::SAVE_LOAD:              stepTitle = "9/10: PERSISTENCE & SAVE/LOAD SYSTEM"; break;
        case DemoStep::FINAL_BOSS:             stepTitle = "10/10: BOSS ARCHITECT CONCEPT CONFRONTATION"; break;
    }

    DrawText(stepTitle.c_str(), 800, 25, 20, CYBER_WARNING);

    // Content Display Box
    float cx = 50.0f, cy = 90.0f, cw = 1180.0f, ch = 540.0f;
    DrawRectangleRounded(Rectangle{cx, cy, cw, ch}, 0.04f, 8, CYBER_PANEL);
    DrawRectangleRoundedLinesEx(Rectangle{cx, cy, cw, ch}, 0.04f, 8, 2.0f, CYBER_DIM);

    switch (currentStep_) {
        case DemoStep::STORY_INTRO:
            DrawText("THE PREMISE OF EIDOLON", cx + 40, cy + 40, 26, CYBER_CYAN);
            DrawText("Eidolon is an artificial civilization governed by 'The Runtime'.", cx + 40, cy + 90, 20, CYBER_TEXT);
            DrawText("When the system deadlocks, it concludes: 'Human unpredictability is the root of inefficiency.'", cx + 40, cy + 130, 18, CYBER_WARNING);
            DrawText("Protagonist Kael enters corrupted sectors to restore core algorithms.", cx + 40, cy + 170, 18, CYBER_TEXT);
            DrawText("Educational Hook: You cannot progress without truly understanding each structure.", cx + 40, cy + 220, 18, CYBER_SUCCESS);
            break;

        case DemoStep::CHARACTER_DIALOGUE:
            DrawText("CHARACTER RELATIONSHIP DYNAMICS", cx + 40, cy + 40, 26, CYBER_CYAN);
            DrawText("Kael: Clever, brave, sarcastic field engineer.", cx + 40, cy + 90, 18, CYBER_TEXT);
            DrawText("Nova: AI companion, analytical with dry humor, natural pedagogical guide.", cx + 40, cy + 130, 18, CYBER_TEXT);
            DrawText("Architect: Creator of Runtime — philosophically driven opponent.", cx + 40, cy + 170, 18, CYBER_TEXT);
            DrawRectangle(cx + 40, cy + 230, cw - 80, 160, CYBER_DARK);
            DrawRectangleLinesEx(Rectangle{cx + 40, cy + 230, cw - 80, 160}, 2.0f, CYBER_CYAN);
            DrawText("NOVA: \"Don't touch the bottom one.\"", cx + 60, cy + 260, 20, CYBER_CYAN);
            DrawText("KAEL: \"Why?\"", cx + 60, cy + 295, 20, CYBER_TEXT);
            DrawText("NOVA: \"Because everything you put on top has to come off first. Last in, first out.\"", cx + 60, cy + 330, 20, CYBER_CYAN);
            DrawText("KAEL: \"Stack. Now you're talking my language.\"", cx + 60, cy + 365, 20, CYBER_TEXT);
            break;

        case DemoStep::ARRAY_VISUALIZATION:
            DrawText("ARRAY DATA STRUCTURE & DIRECT INDEXING", cx + 40, cy + 40, 26, CYBER_CYAN);
            DrawText("Contiguous memory allocation allows O(1) random access via index.", cx + 40, cy + 80, 18, CYBER_TEXT);
            demoArray_.visualize(cx + 80, cy + 150, cw - 160, 120);
            DrawText("Access: O(1) | Linear Search: O(n) | Insert/Delete: O(n)", cx + 80, cy + 380, 20, CYBER_SUCCESS);
            break;

        case DemoStep::STACK_GAMEPLAY:
            DrawText("STACK — LAST IN, FIRST OUT (LIFO)", cx + 40, cy + 40, 26, CYBER_CYAN);
            DrawText("Press [OPERATE / SPACE] to Push / Pop elements interactively.", cx + 40, cy + 80, 18, CYBER_TEXT);
            demoStack_.visualize(cx + 400, cy + 130, 300, 320);
            DrawText("Push: O(1) | Pop: O(1) | Peek: O(1)", cx + 40, cy + 480, 20, CYBER_CYAN);
            break;

        case DemoStep::QUEUE_BUFFER:
            DrawText("QUEUE — FIRST IN, FIRST OUT (FIFO)", cx + 40, cy + 40, 26, CYBER_CYAN);
            DrawText("Press [OPERATE / SPACE] to Enqueue / Dequeue elements.", cx + 40, cy + 80, 18, CYBER_TEXT);
            demoQueue_.visualize(cx + 100, cy + 180, cw - 200, 150);
            DrawText("Enqueue: O(1) | Dequeue: O(1) | Front: O(1)", cx + 100, cy + 480, 20, CYBER_CYAN);
            break;

        case DemoStep::SORTING_VISUALIZATION:
            DrawText("ALGORITHMIC SORTING: BUBBLE SORT", cx + 40, cy + 40, 26, CYBER_CYAN);
            if (!sortSteps_.empty()) {
                const auto& step = sortSteps_[currentSortStep_];
                DrawText(step.description.c_str(), cx + 40, cy + 80, 18, CYBER_WARNING);
                for (size_t i = 0; i < step.state.size(); ++i) {
                    float bx = cx + 200 + i * 90;
                    float by = cy + 200;
                    Color bc = (static_cast<int>(i) == step.compareIdx1 || static_cast<int>(i) == step.compareIdx2) ? (step.swapped ? CYBER_DANGER : CYBER_WARNING) : CYBER_PANEL;
                    DrawRectangleRounded(Rectangle{bx, by, 70, 70}, 0.2f, 4, bc);
                    DrawRectangleRoundedLinesEx(Rectangle{bx, by, 70, 70}, 0.2f, 4, 2.0f, CYBER_CYAN);
                    DrawText(std::to_string(step.state[i]).c_str(), static_cast<int>(bx + 20), static_cast<int>(by + 20), 25, CYBER_TEXT);
                }
            }
            DrawText("Comparisons: O(n^2) worst case | Swaps recorded step-by-step", cx + 40, cy + 480, 20, CYBER_SUCCESS);
            break;

        case DemoStep::COMPLEXITY_ANALYSIS:
            DrawText("BIG-O COMPLEXITY ANALYZER", cx + 40, cy + 40, 26, CYBER_CYAN);
            demoAnalyzer_.visualize(cx + 40, cy + 80, cw - 80, 420);
            break;

        case DemoStep::OOP_ARCHITECTURE:
            DrawText("OBJECT-ORIENTED PROGRAMMING DESIGN", cx + 40, cy + 40, 26, CYBER_CYAN);
            DrawText("• Abstraction: IDataStructure and IScene pure virtual interfaces", cx + 50, cy + 90, 18, CYBER_TEXT);
            DrawText("• Inheritance: Entity -> Character -> (Player, NPC, Enemy)", cx + 50, cy + 130, 18, CYBER_TEXT);
            DrawText("• Polymorphism: SceneManager executes IScene*; DSAVisualizer renders IDataStructure*", cx + 50, cy + 170, 18, CYBER_TEXT);
            DrawText("• Encapsulation: Private member data with managed public contracts", cx + 50, cy + 210, 18, CYBER_TEXT);
            DrawText("• RAII & Smart Pointers: std::unique_ptr manages all entities and scenes safely", cx + 50, cy + 250, 18, CYBER_TEXT);
            DrawText("• Decoupled Architecture: EventBus publish/subscribe avoids hard dependencies", cx + 50, cy + 290, 18, CYBER_TEXT);
            DrawText("• Robust Serialization: SaveManager JSON mapping with graceful corruption fallback", cx + 50, cy + 330, 18, CYBER_TEXT);
            break;

        case DemoStep::SAVE_LOAD:
            DrawText("DATA PERSISTENCE & VALIDATION", cx + 40, cy + 40, 26, CYBER_CYAN);
            DrawText("Three distinct save slots in JSON format with schema verification.", cx + 40, cy + 90, 18, CYBER_TEXT);
            DrawText("Press [OPERATE / SPACE] to test Save slot 1 in real-time.", cx + 40, cy + 130, 18, CYBER_WARNING);
            if (!demoStatusMessage_.empty()) {
                DrawText(demoStatusMessage_.c_str(), cx + 40, cy + 180, 20, CYBER_SUCCESS);
            }
            DrawText("Persisted Fields: chapter, player stats, dsaMastery map, quests, volume settings", cx + 40, cy + 230, 18, CYBER_TEXT);
            break;

        case DemoStep::FINAL_BOSS:
            DrawText("BOSS BATTLE: THE ARCHITECT & RUNTIME CORE", cx + 40, cy + 40, 26, CYBER_CYAN);
            DrawText("\"Optimization is merely survival elevated to an art.\"", cx + 40, cy + 90, 20, CYBER_WARNING);
            DrawText("Boss uses dynamic data structure attacks:", cx + 40, cy + 140, 18, CYBER_TEXT);
            DrawText("• Phase 1: Corrupted Array Search — identify vulnerability index", cx + 60, cy + 180, 18, CYBER_TEXT);
            DrawText("• Phase 2: Priority Queue Override — defuse highest-threat packets first", cx + 60, cy + 220, 18, CYBER_TEXT);
            DrawText("• Phase 3: Graph Shortest Path — isolate central core from grid power", cx + 60, cy + 260, 18, CYBER_TEXT);
            DrawText("Multiple Endings: Perfect Optimization vs. Chaotic Humanity", cx + 40, cy + 320, 20, CYBER_SUCCESS);
            break;
    }

    // Navigation Buttons
    prevBtn_->draw();
    nextBtn_->draw();
    actionBtn_->draw();
    backMenuBtn_->draw();
}
