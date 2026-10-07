#pragma once
#include "core/SceneManager.h"
#include <raylib.h>
#include <string>
#include <vector>

enum class BattleMenuState {
    ROOT_MENU = 0,   // FIGHT, PARTY, BAG, RUN
    SKILL_MENU = 1,  // Dynamic skill list
    PARTY_MENU = 2,  // Switch active Logix
    MESSAGE = 3,     // Turn message banner
    VICTORY = 4,
    DEFEAT = 5
};

struct LogixMember {
    std::string name;
    std::string dsaType;
    int maxHp;
    int hp;
    std::vector<std::string> skills;
    Texture2D sprite{0};
};

class GBABattleScene : public IScene {
public:
    GBABattleScene(std::string enemyName, int enemyHp);
    ~GBABattleScene() override;

    void onEnter() override;
    void onExit() override;
    void update(float dt) override;
    void draw() override;
    std::string getName() const override { return "GBABattleScene"; }

private:
    void executePlayerAction(int skillIdx);
    void executeEnemyTurn();
    void switchLogix(int newIdx);

    std::string enemyName_;
    int enemyMaxHp_ = 80;
    int enemyHp_ = 80;

    std::vector<LogixMember> party_;
    int activeLogixIdx_ = 0;
    int stackCharges_ = 0;
    bool isShielded_ = false;

    BattleMenuState state_ = BattleMenuState::ROOT_MENU;
    int menuCursor_ = 0; // 0..3
    int skillCursor_ = 0;
    int partyCursor_ = 0;

    std::string battleMessage_;
    float messageTimer_ = 0.0f;

    Texture2D enemySprite_{0};
    bool spritesLoaded_ = false;

    // Transition effect
    float introTransition_ = 1.0f;
    bool isTransitioningOut_ = false;
};
