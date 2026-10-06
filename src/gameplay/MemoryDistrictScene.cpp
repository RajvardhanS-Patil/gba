#include "MemoryDistrictScene.h"
#include "core/Game.h"
#include "ui/UISystem.h"
#include "ui/MainMenu.h"
#include "ui/PauseMenu.h"
#include "audio/AudioManager.h"
#include "audio/MusicManager.h"
#include "audio/VoiceManager.h"
#include "utils/Logger.h"
#include "utils/MathUtils.h"
#include <cmath>

MemoryDistrictScene::MemoryDistrictScene() {
    initLevel();
}

void MemoryDistrictScene::initLevel() {
    LOG_INFO("Initializing Memory District level...");

    // Setup player
    player_ = std::make_unique<Player>(200.0f, 300.0f);
    world_.setPlayer(player_.get());

    // Setup Nova companion NPC
    novaNPC_ = std::make_unique<NPC>("nova", "Nova", 260.0f, 300.0f, "ch1_nova_intro_001");

    // Setup enemy (spawns in combat area)
    memoryEater_ = std::make_unique<Enemy>("memory_eater", "Memory Eater", 800.0f, 300.0f, 100, 15, "array_search");

    // Build the room tiles
    nlohmann::json roomJson;
    roomJson["width"] = 40;
    roomJson["height"] = 25;
    std::vector<std::vector<int>> map(25, std::vector<int>(40, 0));
    
    // Add border walls
    for (int x = 0; x < 40; ++x) {
        map[0][x] = 1;
        map[24][x] = 1;
    }
    for (int y = 0; y < 25; ++y) {
        map[y][0] = 1;
        map[y][39] = 1;
    }

    // Add decorative structures & barriers
    for (int x = 12; x < 28; ++x) {
        map[6][x] = (x % 3 == 0) ? 2 : 1;
        map[18][x] = (x % 3 == 0) ? 2 : 1;
    }

    roomJson["map"] = map;
    world_.loadRoom("memory_district_01", roomJson);

    // Add Memory Node interactables in an Array sequence (indices 0..7)
    for (int i = 0; i < 8; ++i) {
        float nx = 350.0f + i * 85.0f;
        float ny = 300.0f;
        auto node = std::make_unique<Interactable>(
            "Node [" + std::to_string(i) + "]",
            "MEMORY_NODE",
            nx, ny, 60.0f, 60.0f
        );
        world_.getCurrentRoom()->addInteractable(std::move(node));
        // Fill our internal ArrayDS structure with memory values
        // Node 4 has the corrupted address 999
        memoryArray_.insert((i == targetCorruptedIndex_) ? 999 : 100 + i * 42);
    }

    // Load dialogue
    dialogueSystem_.loadDialogue("data/dialogue/chapter1.json");

    // Setup HUD
    hud_.setHealth(100, 100);
    hud_.setEnergy(100, 100);
    hud_.setChapter("CHAPTER 1 — MEMORY DISTRICT");
    hud_.setObjective("Investigate corrupted memory node in Sector 7-Alpha");
    hud_.setAbility("SCAN");

    // Init progression
    progressionSystem_.initConcept("Array", 2);
    progressionSystem_.initConcept("Linear Search", 2);

    // Setup chapter quest
    Quest ch1Quest;
    ch1Quest.id = "ch1_main";
    ch1Quest.name = "Sector 7-Alpha Recovery";
    ch1Quest.description = "Identify the corrupted memory cell and stabilize the district.";
    ch1Quest.state = QuestState::ACTIVE;
    ch1Quest.objectives = {
        {"Converse with Nova", false, "talk"},
        {"Perform Linear Search to find corrupted cell", false, "search"},
        {"Neutralize Memory Eater anomaly", false, "combat"}
    };
    ch1Quest.rewardItemId = "ARRAY_CORE";
    ch1Quest.rewardXP = 250;
    questSystem_.addQuest(ch1Quest);
}

void MemoryDistrictScene::onEnter() {
    LOG_INFO("Entered Memory District Scene");
    // Start story intro dialogue
    triggerDialogue("ch1_intro_001");
}

void MemoryDistrictScene::onExit() {
    LOG_INFO("Exiting Memory District Scene");
}

void MemoryDistrictScene::triggerDialogue(const std::string& nodeId) {
    dialogueSystem_.startDialogue(nodeId);
    state_ = State::DIALOGUE;
}

void MemoryDistrictScene::checkInteractions() {
    Vector2 playerPos = player_->getPosition();

    // Check interaction with Nova
    if (novaNPC_->isInRange(playerPos)) {
        hud_.showInteractionPrompt(true);
        if (IsKeyPressed(KEY_E)) {
            triggerDialogue(novaNPC_->getDialogueId());
        }
        return;
    }

    // Check interaction with Memory Nodes
    auto room = world_.getCurrentRoom();
    if (room) {
        for (int i = 0; i < 8; ++i) {
            float nx = 350.0f + i * 85.0f;
            float ny = 300.0f;
            Vector2 nodeCenter = {nx + 30.0f, ny + 30.0f};
            if (MathUtils::distance(playerPos, nodeCenter) < 55.0f) {
                hud_.showInteractionPrompt(true);
                if (IsKeyPressed(KEY_E)) {
                    memoryArray_.setHighlight(i);
                    if (i == targetCorruptedIndex_ && !memoryNodeFound_) {
                        memoryNodeFound_ = true;
                        progressionSystem_.recordSuccess("Array");
                        progressionSystem_.recordSuccess("Linear Search");
                        questSystem_.completeObjective("ch1_main", 1);
                        bannerText_ = "CORRUPTED NODE LOCATED! (Index 4: Address 999)";
                        bannerTimer_ = 4.0f;
                        triggerDialogue("ch1_memory_found");
                    } else if (!memoryNodeFound_) {
                        bannerText_ = "Checking Node [" + std::to_string(i) + "]... Value normal.";
                        bannerTimer_ = 2.0f;
                    }
                }
                return;
            }
        }
    }

    hud_.showInteractionPrompt(false);
}

void MemoryDistrictScene::update(float dt) {
    if (bannerTimer_ > 0.0f) bannerTimer_ -= dt;

    // Toggle architecture inspector (viva mode)
    if (IsKeyPressed(KEY_F1) || IsKeyPressed(KEY_V)) {
        showArchitectureInspector_ = !showArchitectureInspector_;
    }

    // Toggle visualizer panel
    if (IsKeyPressed(KEY_TAB)) {
        showVisualizer_ = !showVisualizer_;
    }

    // Pause menu
    if (IsKeyPressed(KEY_ESCAPE) && state_ != State::DIALOGUE) {
        Game::getSceneManager().pushScene(std::make_unique<PauseMenu>());
        return;
    }

    switch (state_) {
        case State::DIALOGUE: {
            bool isComplete = dialogueSystem_.isTypewriterComplete();
            std::vector<std::string> choices;
            auto currNode = dialogueSystem_.getCurrentNode();
            if (currNode) {
                for (const auto& c : currNode->choices) choices.push_back(c.text);
            }
            int selectedChoice = -1;
            dialogueUI_.update(dt, dialogueSystem_.getDisplayedText(), isComplete, choices, selectedChoice);
            
            if (selectedChoice >= 0) {
                dialogueSystem_.selectChoice(selectedChoice);
            }

            if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
                if (!isComplete) {
                    dialogueSystem_.skipTypewriter();
                } else if (choices.empty()) {
                    dialogueSystem_.advanceDialogue();
                    if (!dialogueSystem_.isActive()) {
                        // Check if dialogue ended with specific triggers
                        if (memoryNodeFound_ && !arrayCoreUnlocked_) {
                            arrayCoreUnlocked_ = true;
                            hud_.setAbility("ARRAY CORE");
                            inventorySystem_.addItem({"ARRAY_CORE", "Array Core", "Direct indexing ability (O(1) access).", "core_array", 1});
                            // Next trigger enemy encounter
                            state_ = State::EXPLORATION;
                            triggerDialogue("ch1_enemy_intro");
                        } else if (arrayCoreUnlocked_ && !enemyDefeated_) {
                            state_ = State::COMBAT;
                            combatSystem_.startCombat(player_.get(), memoryEater_.get());
                        } else if (enemyDefeated_) {
                            state_ = State::CHAPTER_COMPLETE;
                        } else {
                            state_ = State::EXPLORATION;
                        }
                    }
                }
            }
            break;
        }

        case State::EXPLORATION: {
            world_.update(dt);
            player_->update(dt);
            novaNPC_->update(dt);
            checkInteractions();
            break;
        }

        case State::COMBAT: {
            combatSystem_.update(dt);
            // Simulate DSA combat choice: Press [1] for Array Linear Scan
            if (combatSystem_.getState() == CombatState::PLAYER_TURN) {
                if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_ENTER)) {
                    combatSystem_.playerAttack(50);
                    bannerText_ = "ARRAY SCAN: Weakness targeted at Index 0!";
                    bannerTimer_ = 2.5f;
                }
            }
            if (combatSystem_.getState() == CombatState::VICTORY) {
                enemyDefeated_ = true;
                questSystem_.completeObjective("ch1_main", 2);
                progressionSystem_.recordSuccess("Array");
                triggerDialogue("ch1_victory");
            }
            break;
        }

        case State::CHAPTER_COMPLETE: {
            if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
                // Save game progress
                SaveData save;
                save.chapter = 2;
                save.playerHealth = player_->getHealth();
                save.unlockedAbilities.push_back("ARRAY_CORE");
                save.dsaMastery["Array"] = 100.0f;
                save.dsaMastery["Linear Search"] = 100.0f;
                saveManager_.save(1, save);
                // Return to main menu
                Game::getSceneManager().changeScene(std::make_unique<MainMenu>());
            }
            break;
        }

        default:
            state_ = State::EXPLORATION;
            break;
    }

    if (showVisualizer_) {
        dsaVisualizer_.update(&memoryArray_);
    }

    hud_.update(dt);
}

void MemoryDistrictScene::draw() {
    ClearBackground(CYBER_DARK);

    if (state_ == State::COMBAT) {
        combatSystem_.draw();
    } else {
        // Draw World & Entities
        world_.draw();
        novaNPC_->draw();
    }

    // Draw Array Structure Overlay in World
    if (state_ == State::EXPLORATION || state_ == State::DIALOGUE) {
        float arrayVisX = 280.0f;
        float arrayVisY = 80.0f;
        DrawText("SECTOR 7-ALPHA MEMORY BUFFER [ARRAY]:", static_cast<int>(arrayVisX), static_cast<int>(arrayVisY - 25), 18, CYBER_CYAN);
        memoryArray_.visualize(arrayVisX, arrayVisY, 720.0f, 90.0f);
    }

    // Draw HUD
    hud_.draw();

    // Draw On-Screen Banners
    if (bannerTimer_ > 0.0f) {
        int tw = MeasureText(bannerText_.c_str(), 20);
        DrawRectangle(1280/2 - tw/2 - 20, 180, tw + 40, 40, CYBER_PANEL);
        DrawRectangleLines(1280/2 - tw/2 - 20, 180, tw + 40, 40, CYBER_CYAN);
        DrawText(bannerText_.c_str(), 1280/2 - tw/2, 190, 20, CYBER_CYAN);
    }

    // Draw Dialogue if active
    if (state_ == State::DIALOGUE && dialogueSystem_.isActive()) {
        auto node = dialogueSystem_.getCurrentNode();
        std::string speaker = node ? node->speaker : "UNKNOWN";
        std::vector<std::string> choices;
        if (node) {
            for (const auto& c : node->choices) choices.push_back(c.text);
        }
        bool isComp = dialogueSystem_.isTypewriterComplete();
        dialogueUI_.draw(speaker, dialogueSystem_.getDisplayedText(), isComp, choices);
    }

    // Draw Chapter Completion Screen
    if (state_ == State::CHAPTER_COMPLETE) {
        DrawRectangle(0, 0, 1280, 720, Color{10, 14, 23, 230});
        DrawText("CHAPTER 1 COMPLETE", 1280/2 - MeasureText("CHAPTER 1 COMPLETE", 40)/2, 200, 40, CYBER_SUCCESS);
        DrawText("ARRAY CORE ACQUIRED", 1280/2 - MeasureText("ARRAY CORE ACQUIRED", 25)/2, 270, 25, CYBER_CYAN);
        DrawText("Mastery: ARRAY (100%) | LINEAR SEARCH (100%)", 1280/2 - MeasureText("Mastery: ARRAY (100%) | LINEAR SEARCH (100%)", 20)/2, 330, 20, CYBER_TEXT);
        DrawText("Access: O(1) by Index | Search: O(n) Unsorted", 1280/2 - MeasureText("Access: O(1) by Index | Search: O(n) Unsorted", 20)/2, 370, 20, CYBER_DIM);
        DrawText("Press [SPACE] or [ENTER] to Return to Menu", 1280/2 - MeasureText("Press [SPACE] or [ENTER] to Return to Menu", 20)/2, 500, 20, CYBER_TEXT);
    }

    // Draw Visualizer overlay
    if (showVisualizer_) {
        dsaVisualizer_.draw(&memoryArray_, 140, 80, 1000, 560);
    }

    // Draw Architecture Inspector (Viva Mode)
    if (showArchitectureInspector_) {
        drawArchitectureInspector();
    }

    // Hint in bottom-left
    DrawText("[TAB] Visualizer  |  [F1/V] Viva Inspector  |  [ESC] Pause", 20, 695, 12, CYBER_DIM);
}

void MemoryDistrictScene::drawArchitectureInspector() {
    float ix = 820.0f;
    float iy = 20.0f;
    float iw = 440.0f;
    float ih = 680.0f;

    DrawRectangleRounded({ix, iy, iw, ih}, 0.05f, 8, Color{15, 20, 32, 245});
    DrawRectangleRoundedLinesEx({ix, iy, iw, ih}, 0.05f, 8, 2.0f, CYBER_CYAN);

    DrawText("ARCHITECTURE INSPECTOR (VIVA MODE)", static_cast<int>(ix + 20), static_cast<int>(iy + 20), 18, CYBER_CYAN);
    DrawLine(static_cast<int>(ix + 20), static_cast<int>(iy + 45), static_cast<int>(ix + iw - 20), static_cast<int>(iy + 45), CYBER_DIM);

    int y = static_cast<int>(iy + 60);
    auto printSection = [&](const char* title, const char* content) {
        DrawText(title, static_cast<int>(ix + 20), y, 14, CYBER_WARNING);
        y += 20;
        DrawText(content, static_cast<int>(ix + 30), y, 13, CYBER_TEXT);
        y += 30;
    };

    printSection("ACTIVE SCENE:", "MemoryDistrictScene : public IScene");
    printSection("POLYMORPHISM DEMO:", "IScene* updated/drawn via SceneManager\nIDataStructure* visualized via DSAVisualizer\nEntity* polymorphic update/draw in Room");
    printSection("INHERITANCE HIERARCHY:", "Entity -> Character -> Player / Enemy / NPC\nIDataStructure -> ArrayDS, Stack, Queue");
    printSection("CURRENT DATA STRUCTURE:", "ArrayDS (Sequential contiguous buffer)\nSize: 8 elements | Capacity: 8");
    printSection("ACTIVE ALGORITHMS:", "Linear Search (Corrupted memory scan)\nDirect Indexing Access (O(1) retrieval)");
    printSection("TIME COMPLEXITY:", "Access: O(1) | Search: O(n)\nInsert: O(n) | Delete: O(n)");
    printSection("ENCAPSULATION & RAII:", "std::unique_ptr<Entity>, smart pointers\nPrivate members with accessors\nSaveManager JSON serialization");
    printSection("MEMORY ALLOCATION:", "Dynamic entities managed via unique_ptr\nTexture & Audio device lifecycle managed");
}
