#include "GBABattleScene.h"
#include "core/GBAEngine.h"
#include "core/Game.h"
#include "audio/SFXManager.h"
#include "audio/MusicManager.h"
#include <cmath>
#include <algorithm>

GBABattleScene::GBABattleScene(std::string enemyName, int enemyHp)
    : enemyName_(enemyName), enemyMaxHp_(enemyHp), enemyHp_(enemyHp)
{
    Texture2D arrixTex{0};
    Texture2D linkoTex{0};
    Texture2D stackraTex{0};

    if (FileExists("assets/sprites/arrix.png")) {
        arrixTex = LoadTexture("assets/sprites/arrix.png");
    }
    if (FileExists("assets/sprites/linko.png")) {
        linkoTex = LoadTexture("assets/sprites/linko.png");
    }
    if (FileExists("assets/sprites/stackra.png")) {
        stackraTex = LoadTexture("assets/sprites/stackra.png");
    }
    if (FileExists("assets/sprites/glitch_node.png")) {
        enemySprite_ = LoadTexture("assets/sprites/glitch_node.png");
    }
    spritesLoaded_ = true;

    // Initialize 3 Logix party members
    party_.push_back({
        "ARRIX", "ARRAY", 100, 100,
        {"1. INDEX STRIKE [O(1)]", "2. LINEAR SCAN  [O(n)]", "3. CACHE BUFFER [DEF]"},
        arrixTex
    });

    party_.push_back({
        "LINKO", "LIST", 90, 90,
        {"1. POINTER BYPASS [O(1)]", "2. NODE SPLICE [HEAL]", "3. HEAD REWIRE [DEF]"},
        linkoTex
    });

    party_.push_back({
        "STACKRA", "STACK", 110, 110,
        {"1. PUSH CHARGE [O(1)]", "2. LIFO BURST  [CRIT]", "3. PEEK SCAN [O(1)]"},
        stackraTex
    });

    activeLogixIdx_ = 0;
}

GBABattleScene::~GBABattleScene() {
    if (spritesLoaded_) {
        for (auto& m : party_) {
            if (m.sprite.id > 0) UnloadTexture(m.sprite);
        }
        if (enemySprite_.id > 0) UnloadTexture(enemySprite_);
        spritesLoaded_ = false;
    }
}

void GBABattleScene::onEnter() {
    introTransition_ = 1.0f;
    isTransitioningOut_ = false;
    state_ = BattleMenuState::ROOT_MENU;
    battleMessage_ = "Wild " + enemyName_ + " appeared!";
    SFXManager::instance().play("scan");
    MusicManager::instance().play("assets/music/battle.wav", true);
}

void GBABattleScene::onExit() {}

void GBABattleScene::switchLogix(int newIdx) {
    if (newIdx < 0 || newIdx >= static_cast<int>(party_.size())) return;
    activeLogixIdx_ = newIdx;
    state_ = BattleMenuState::MESSAGE;
    battleMessage_ = "Swapped to " + party_[activeLogixIdx_].name + "! [" + party_[activeLogixIdx_].dsaType + "]";
    messageTimer_ = 1.4f;
    SFXManager::instance().play("click");
}

void GBABattleScene::update(float dt) {
    MusicManager::instance().update(dt);

    if (introTransition_ > 0.0f) {
        introTransition_ -= dt * 3.5f;
        if (introTransition_ < 0.0f) introTransition_ = 0.0f;
        return;
    }

    if (messageTimer_ > 0.0f) {
        messageTimer_ -= dt;
        if (messageTimer_ <= 0.0f) {
            if (state_ == BattleMenuState::VICTORY || state_ == BattleMenuState::DEFEAT) {
                Game::getSceneManager().popScene();
                return;
            } else {
                state_ = BattleMenuState::ROOT_MENU;
            }
        }
        return;
    }

    if (state_ == BattleMenuState::ROOT_MENU) {
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
            menuCursor_ = (menuCursor_ - 2 + 4) % 4;
            SFXManager::instance().play("hover");
        } else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
            menuCursor_ = (menuCursor_ + 2) % 4;
            SFXManager::instance().play("hover");
        } else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
            menuCursor_ = (menuCursor_ % 2 == 1) ? menuCursor_ - 1 : menuCursor_ + 1;
            SFXManager::instance().play("hover");
        } else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
            menuCursor_ = (menuCursor_ % 2 == 0) ? menuCursor_ + 1 : menuCursor_ - 1;
            SFXManager::instance().play("hover");
        }

        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_ENTER)) {
            SFXManager::instance().play("click");
            if (menuCursor_ == 0) { // FIGHT
                state_ = BattleMenuState::SKILL_MENU;
                skillCursor_ = 0;
            } else if (menuCursor_ == 1) { // PARTY
                state_ = BattleMenuState::PARTY_MENU;
                partyCursor_ = activeLogixIdx_;
            } else if (menuCursor_ == 2) { // BAG
                battleMessage_ = "Used REPAIR PATCH: +35 HP restored!";
                party_[activeLogixIdx_].hp = std::min(party_[activeLogixIdx_].maxHp, party_[activeLogixIdx_].hp + 35);
                messageTimer_ = 1.4f;
                state_ = BattleMenuState::MESSAGE;
                SFXManager::instance().play("success");
            } else if (menuCursor_ == 3) { // RUN
                battleMessage_ = "Got away safely!";
                state_ = BattleMenuState::VICTORY;
                messageTimer_ = 1.2f;
            }
        }
    } else if (state_ == BattleMenuState::SKILL_MENU) {
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
            skillCursor_ = (skillCursor_ - 1 + 3) % 3;
            SFXManager::instance().play("hover");
        } else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
            skillCursor_ = (skillCursor_ + 1) % 3;
            SFXManager::instance().play("hover");
        } else if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_X)) {
            state_ = BattleMenuState::ROOT_MENU;
            SFXManager::instance().play("click");
        }

        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_ENTER)) {
            executePlayerAction(skillCursor_);
        }
    } else if (state_ == BattleMenuState::PARTY_MENU) {
        int nMembers = static_cast<int>(party_.size());
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
            partyCursor_ = (partyCursor_ - 1 + nMembers) % nMembers;
            SFXManager::instance().play("hover");
        } else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
            partyCursor_ = (partyCursor_ + 1) % nMembers;
            SFXManager::instance().play("hover");
        } else if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_X)) {
            state_ = BattleMenuState::ROOT_MENU;
            SFXManager::instance().play("click");
        }

        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_ENTER)) {
            if (partyCursor_ != activeLogixIdx_) {
                switchLogix(partyCursor_);
            } else {
                state_ = BattleMenuState::ROOT_MENU;
            }
        }
    }
}

void GBABattleScene::executePlayerAction(int skillIdx) {
    state_ = BattleMenuState::MESSAGE;
    auto& active = party_[activeLogixIdx_];

    if (active.name == "ARRIX") {
        if (skillIdx == 0) { // INDEX STRIKE
            SFXManager::instance().play("hit");
            int dmg = 45;
            enemyHp_ -= dmg;
            battleMessage_ = "ARRIX used INDEX STRIKE! (O(1): -45 HP)";
        } else if (skillIdx == 1) { // LINEAR SCAN
            SFXManager::instance().play("hit");
            int dmg = 25;
            enemyHp_ -= dmg;
            battleMessage_ = "ARRIX used LINEAR SCAN! (O(n): -25 HP)";
        } else { // CACHE BUFFER
            SFXManager::instance().play("click");
            isShielded_ = true;
            battleMessage_ = "ARRIX loaded CACHE BUFFER! Next damage blocked.";
        }
    } else if (active.name == "LINKO") {
        if (skillIdx == 0) { // POINTER BYPASS
            SFXManager::instance().play("hit");
            int dmg = 40;
            enemyHp_ -= dmg;
            battleMessage_ = "LINKO used POINTER BYPASS! (-40 HP unblocked)";
        } else if (skillIdx == 1) { // NODE SPLICE
            SFXManager::instance().play("hit");
            int dmg = 20;
            enemyHp_ -= dmg;
            active.hp = std::min(active.maxHp, active.hp + 25);
            battleMessage_ = "NODE SPLICE! -20 enemy HP, restored +25 HP!";
        } else { // HEAD REWIRE
            SFXManager::instance().play("click");
            isShielded_ = true;
            battleMessage_ = "LINKO rewired HEAD pointer! Deflecting hit.";
        }
    } else if (active.name == "STACKRA") {
        if (skillIdx == 0) { // PUSH CHARGE
            SFXManager::instance().play("click");
            stackCharges_++;
            battleMessage_ = TextFormat("STACKRA PUSHED charge! Stack depth: %d", stackCharges_);
        } else if (skillIdx == 1) { // LIFO BURST
            SFXManager::instance().play("hit");
            int dmg = 35 + stackCharges_ * 30;
            enemyHp_ -= dmg;
            battleMessage_ = TextFormat("LIFO BURST popped %d nodes! (-%d HP CRIT!)", stackCharges_, dmg);
            stackCharges_ = 0;
        } else { // PEEK SCAN
            SFXManager::instance().play("hit");
            enemyHp_ -= 20;
            battleMessage_ = "STACKRA PEEKED top instruction! (-20 HP)";
        }
    }

    messageTimer_ = 1.6f;

    if (enemyHp_ <= 0) {
        enemyHp_ = 0;
        state_ = BattleMenuState::VICTORY;
        battleMessage_ = enemyName_ + " solved and stabilized! VICTORY!";
        SFXManager::instance().play("success");
    } else {
        executeEnemyTurn();
    }
}

void GBABattleScene::executeEnemyTurn() {
    int enemyDmg = 15;
    if (isShielded_) {
        enemyDmg = 0;
        isShielded_ = false;
    }
    party_[activeLogixIdx_].hp -= enemyDmg;
    if (party_[activeLogixIdx_].hp <= 0) {
        party_[activeLogixIdx_].hp = 0;
        state_ = BattleMenuState::DEFEAT;
        battleMessage_ = party_[activeLogixIdx_].name + " crashed! Critical fault...";
    }
}

void GBABattleScene::draw() {
    // 1. Handheld 240x160 arena background
    ClearBackground(GBA_DARK_BLUE);

    // Subtle arena battle platforms (ellipses)
    DrawEllipse(176, 56, 42, 14, GBA_MID_BLUE);   // Enemy platform
    DrawEllipse(64, 106, 50, 16, GBA_MID_BLUE);   // Player platform

    // 2. Enemy Sprite & Stat Card
    if (enemySprite_.id > 0) {
        DrawTexture(enemySprite_, 158, 26, WHITE);
    }
    // Enemy HP Box (Top-Left)
    DrawRectangleRounded(Rectangle{14, 12, 110, 26}, 0.2f, 4, GBA_CREAM);
    DrawRectangleRoundedLinesEx(Rectangle{14, 12, 110, 26}, 0.2f, 4, 1.2f, GBA_FRAME_DARK);
    DrawText(enemyName_.c_str(), 20, 16, 9, GBA_BLACK);
    // Enemy HP Bar
    float eHpPct = static_cast<float>(enemyHp_) / enemyMaxHp_;
    DrawRectangle(50, 28, 66, 5, GBA_BLACK);
    DrawRectangle(50, 28, static_cast<int>(66 * eHpPct), 5, (eHpPct > 0.3f) ? GBA_GREEN_MID : GBA_CRIMSON);
    DrawText("HP", 34, 27, 7, GBA_BLACK);

    // 3. Active Player Logix Sprite & Stat Card
    const auto& active = party_[activeLogixIdx_];
    if (active.sprite.id > 0) {
        DrawTexture(active.sprite, 46, 74, WHITE);
    }

    // Player HP Box (Right-Mid)
    DrawRectangleRounded(Rectangle{116, 66, 114, 32}, 0.2f, 4, GBA_CREAM);
    DrawRectangleRoundedLinesEx(Rectangle{116, 66, 114, 32}, 0.2f, 4, 1.2f, GBA_FRAME_DARK);
    DrawText(active.name.c_str(), 122, 70, 9, GBA_BLACK);
    DrawText(TextFormat("[%s]", active.dsaType.c_str()), 172, 70, 8, GBA_MID_BLUE);

    // Player HP Bar
    float pHpPct = static_cast<float>(active.hp) / active.maxHp;
    DrawRectangle(150, 82, 70, 5, GBA_BLACK);
    DrawRectangle(150, 82, static_cast<int>(70 * pHpPct), 5, (pHpPct > 0.3f) ? GBA_GREEN_MID : GBA_CRIMSON);
    DrawText("HP", 134, 81, 7, GBA_BLACK);
    DrawText(TextFormat("%d/%d", active.hp, active.maxHp), 182, 89, 7, GBA_BLACK);

    // 4. Classic 240x160 Bottom Command Box (y: 106..156)
    Rectangle cmdBox = {4.0f, 106.0f, 232.0f, 50.0f};
    DrawRectangleRounded(cmdBox, 0.1f, 4, GBA_CREAM);
    DrawRectangleRoundedLinesEx(cmdBox, 0.1f, 4, 1.5f, GBA_DARK_BLUE);

    if (state_ == BattleMenuState::MESSAGE || messageTimer_ > 0.0f) {
        DrawText(battleMessage_.c_str(), 12, 122, 9, GBA_BLACK);
    } else if (state_ == BattleMenuState::ROOT_MENU) {
        DrawText("What will", 14, 116, 9, GBA_BLACK);
        DrawText(TextFormat("%s do?", active.name.c_str()), 14, 130, 9, GBA_BLACK);

        // 2x2 Menu on Right
        Rectangle menuCard = {118.0f, 109.0f, 114.0f, 44.0f};
        DrawRectangleRounded(menuCard, 0.15f, 4, GBA_WHITE);
        DrawRectangleRoundedLinesEx(menuCard, 0.15f, 4, 1.0f, GBA_MID_BLUE);

        const char* opts[4] = {"FIGHT", "PARTY", "BAG", "RUN"};
        int ox[4] = {134, 182, 134, 182};
        int oy[4] = {116, 116, 133, 133};

        for (int i = 0; i < 4; ++i) {
            DrawText(opts[i], ox[i], oy[i], 9, GBA_BLACK);
        }

        // Pointer triangle cursor
        int cx = ox[menuCursor_] - 8;
        int cy = oy[menuCursor_] + 2;
        DrawTriangle(
            Vector2{static_cast<float>(cx), static_cast<float>(cy)},
            Vector2{static_cast<float>(cx), static_cast<float>(cy + 6)},
            Vector2{static_cast<float>(cx + 5), static_cast<float>(cy + 3)},
            GBA_CRIMSON
        );
    } else if (state_ == BattleMenuState::SKILL_MENU) {
        for (size_t i = 0; i < active.skills.size(); ++i) {
            DrawText(active.skills[i].c_str(), 24, 112 + static_cast<int>(i) * 12, 8, GBA_BLACK);
        }

        int scy = 112 + skillCursor_ * 12 + 2;
        DrawTriangle(
            Vector2{14.0f, static_cast<float>(scy)},
            Vector2{14.0f, static_cast<float>(scy + 6)},
            Vector2{19.0f, static_cast<float>(scy + 3)},
            GBA_CRIMSON
        );
    } else if (state_ == BattleMenuState::PARTY_MENU) {
        DrawText("LOGIX CREW // CHOOSE ACTIVE:", 12, 110, 8, GBA_MID_BLUE);
        for (size_t i = 0; i < party_.size(); ++i) {
            int py = 120 + static_cast<int>(i) * 11;
            bool isCurrent = (static_cast<int>(i) == activeLogixIdx_);
            const char* label = TextFormat("%s [%s] HP:%d/%d %s",
                party_[i].name.c_str(), party_[i].dsaType.c_str(),
                party_[i].hp, party_[i].maxHp,
                isCurrent ? "(ACTIVE)" : "");
            DrawText(label, 24, py, 8, isCurrent ? GBA_DARK_BLUE : GBA_BLACK);
        }

        int pcy = 120 + partyCursor_ * 11 + 2;
        DrawTriangle(
            Vector2{14.0f, static_cast<float>(pcy)},
            Vector2{14.0f, static_cast<float>(pcy + 6)},
            Vector2{19.0f, static_cast<float>(pcy + 3)},
            GBA_CRIMSON
        );
    }

    // Battle intro screen wipe
    if (introTransition_ > 0.0f) {
        int barHeight = static_cast<int>(160 * introTransition_);
        DrawRectangle(0, 0, 240, barHeight, GBA_BLACK);
    }
}
