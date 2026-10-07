#pragma once
#include "core/SceneManager.h"
#include "world/Tilemap.h"
#include "entities/Player.h"
#include "ui/GBADialogueUI.h"
#include "ui/CodedexCodeEditorUI.h"
#include <memory>
#include <string>
#include <vector>

class GBAOverworldScene : public IScene {
public:
    GBAOverworldScene(const std::string& initialMap = "nova_town", int spawnX = 8, int spawnY = 8);
    ~GBAOverworldScene() override;

    void onEnter() override;
    void onExit() override;
    void update(float dt) override;
    void draw() override;
    void drawScreenOverlay() override;
    std::string getName() const override { return "GBAOverworldScene"; }

    void switchMap(const std::string& mapName, int spawnX, int spawnY);

private:
    void handleInteraction();
    void checkEncounter();
    void applyPersistentState();
    void updateBGM();

    std::string currentMapName_ = "nova_town";
    std::unique_ptr<Tilemap> currentMap_;
    std::unique_ptr<Player> player_;
    Camera2D camera_{0};

    GBADialogueUI dialogueUI_;
    CodedexCodeEditorUI codeEditorUI_;

    // Nova companion floating adjacent to Kael
    Vector2 novaPos_{0, 0};
    Texture2D novaSprite_{0};
    bool novaLoaded_ = false;

    // Scholar NPC in Nova Town
    int scholarTileX_ = 10;
    int scholarTileY_ = 7;
    Texture2D scholarSprite_{0};
    bool scholarLoaded_ = false;

    // Guardian Demon System
    struct GuardianDemon {
        std::string id;
        std::string name;
        std::string title;
        std::string map;
        int tileX = 0;
        int tileY = 0;
        int extraBlockX = -1;
        int extraBlockY = -1;
        std::string spritePath;
        Texture2D sprite{0};
        bool loaded = false;
        bool defeated = false;
        int hp = 100;
        int maxHp = 100;
        float defeatAnim = 0.0f;
    };

    void initDemons();
    GuardianDemon* getFacingDemon(int fx, int fy);
    bool isTileBlockedByDemon(int tx, int ty);
    void fightDemon(GuardianDemon* demon);
    std::vector<GuardianDemon> demons_;

    // Living World: Ambient Particles
    struct AmbientParticle {
        Vector2 pos;
        Vector2 vel;
        float life;
        float maxLife;
        Color color;
        float size;
    };
    std::vector<AmbientParticle> particles_;
    void updateParticles(float dt);
    void drawParticles() const;

    // Living World: Wandering NPCs
    struct WanderingNPC {
        std::string id;
        std::string name;
        std::string map;
        int tileX = 0;
        int tileY = 0;
        int targetX = 0;
        int targetY = 0;
        float moveTimer = 0.0f;
        float idleTimer = 1.0f;
        bool isMoving = false;
        std::string text1;
        std::string text2;
        Texture2D sprite{0};
        bool loaded = false;
    };
    std::vector<WanderingNPC> wanderingNPCs_;
    void initWanderingNPCs();
    void updateNPCs(float dt);
    void drawNPCs() const;
    const WanderingNPC* getFacingNPC(int fx, int fy) const;

    // HUD Quest Tracker
    void drawHUD() const;

    // Quest & DSA world progression flags
    int storyState_ = 0;
    float grassEncounterAccum_ = 0.0f;

    bool novaGateUnlocked_ = false;
    bool novaChestOpened_ = false;
    bool routeBridgeUnlocked_ = false;
    bool routeChestOpened_ = false;
    bool cavernBridgeUnlocked_ = false;
    bool cavernQueueDoorUnlocked_ = false;
    bool cavernChestOpened_ = false;
    bool citadelChestOpened_ = false;
    bool groveChestOpened_ = false;
    bool hashChestOpened_ = false;
    bool heapChestOpened_ = false;
    bool graphChestOpened_ = false;
    bool memoChestOpened_ = false;
    bool sanctumMasterCleared_ = false;
};
