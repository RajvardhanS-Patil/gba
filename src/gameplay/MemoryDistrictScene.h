#pragma once
#include "core/SceneManager.h"
#include "entities/Player.h"
#include "entities/Enemy.h"
#include "entities/NPC.h"
#include "world/World.h"
#include "world/Interactable.h"
#include "dialogue/DialogueSystem.h"
#include "gameplay/PuzzleSystem.h"
#include "gameplay/CombatSystem.h"
#include "gameplay/ProgressionSystem.h"
#include "gameplay/QuestSystem.h"
#include "gameplay/InventorySystem.h"
#include "dsa/ArrayDS.h"
#include "ui/HUD.h"
#include "ui/DialogueUI.h"
#include "ui/DSAVisualizer.h"
#include "save/SaveManager.h"
#include <memory>

class MemoryDistrictScene : public IScene {
public:
    MemoryDistrictScene();
    ~MemoryDistrictScene() override = default;

    void onEnter() override;
    void onExit() override;
    void update(float dt) override;
    void draw() override;
    std::string getName() const override { return "MemoryDistrictScene"; }

private:
    void initLevel();
    void triggerDialogue(const std::string& nodeId);
    void checkInteractions();
    void drawArchitectureInspector();

    // Scene state
    enum class State {
        STORY_INTRO,
        EXPLORATION,
        DIALOGUE,
        PUZZLE,
        COMBAT,
        CHAPTER_COMPLETE
    };
    State state_ = State::STORY_INTRO;

    // Systems
    World world_;
    std::unique_ptr<Player> player_;
    std::unique_ptr<NPC> novaNPC_;
    std::unique_ptr<Enemy> memoryEater_;
    
    DialogueSystem dialogueSystem_;
    DialogueUI dialogueUI_;
    
    PuzzleSystem puzzleSystem_;
    CombatSystem combatSystem_;
    ProgressionSystem progressionSystem_;
    QuestSystem questSystem_;
    InventorySystem inventorySystem_;
    SaveManager saveManager_;

    HUD hud_;
    ArrayDS memoryArray_;
    DSAVisualizer dsaVisualizer_;

    // Gameplay flags
    bool showVisualizer_ = false;
    bool showArchitectureInspector_ = false;
    bool memoryNodeFound_ = false;
    bool arrayCoreUnlocked_ = false;
    bool enemyDefeated_ = false;

    // Interaction tracking
    int targetCorruptedIndex_ = 4; // Node 4 has the corrupted sector
    float bannerTimer_ = 0.0f;
    std::string bannerText_ = "";
};
