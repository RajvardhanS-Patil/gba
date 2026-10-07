#include "CombatSystem.h"
#include <raylib.h>
#include "../utils/Logger.h"
#include "ui/UISystem.h"
#include <cmath>

CombatSystem::CombatSystem() : state_(CombatState::INACTIVE), player_(nullptr), enemy_(nullptr), turnTimer_(0.0f) {}

void CombatSystem::startCombat(Player* player, Enemy* enemy) {
    player_ = player;
    enemy_ = enemy;
    state_ = CombatState::PLAYER_TURN;
    turnTimer_ = 0.0f;
    LOG_INFO("Combat started with " + enemy->getId());
}

void CombatSystem::update(float dt) {
    if (state_ == CombatState::INACTIVE) return;

    if (player_ && !player_->isAlive()) {
        state_ = CombatState::DEFEAT;
        turnTimer_ += dt;
        if (turnTimer_ > 2.5f) endCombat(false);
        return;
    }

    if (enemy_ && !enemy_->isAlive()) {
        state_ = CombatState::VICTORY;
        turnTimer_ += dt;
        if (turnTimer_ > 2.5f) endCombat(true);
        return;
    }

    if (state_ == CombatState::ENEMY_TURN) {
        turnTimer_ += dt;
        if (turnTimer_ > 1.2f) {
            if (player_ && enemy_) {
                player_->takeDamage(enemy_->getDamage());
            }
            state_ = CombatState::PLAYER_TURN;
            turnTimer_ = 0.0f;
        }
    }
}

void CombatSystem::draw() {
    if (state_ == CombatState::INACTIVE) return;

    float time = (float)GetTime();
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    // Dark cyberpunk arena backdrop
    DrawRectangle(0, 0, sw, sh, Color{8, 12, 20, 235});
    
    // Background digital grid
    for (int x = 0; x < sw; x += 40) DrawLine(x, 0, x, sh, Color{0, 229, 255, 15});
    for (int y = 0; y < sh; y += 40) DrawLine(0, y, sw, y, Color{0, 229, 255, 15});

    // Header Arena Banner
    DrawRectangle(0, 40, sw, 50, Color{15, 22, 35, 200});
    DrawLine(0, 40, sw, 40, CYBER_CYAN);
    DrawLine(0, 90, sw, 90, CYBER_CYAN);
    DrawText("CRITICAL ANOMALY ENGAGEMENT: MEMORY EATER DETECTED", sw/2 - MeasureText("CRITICAL ANOMALY ENGAGEMENT: MEMORY EATER DETECTED", 20)/2, 55, 20, CYBER_WARNING);

    // ─── PLAYER STATUS CARD (Left) ───
    float px = 100;
    float py = 160;
    DrawRectangleRounded({px, py, 340, 240}, 0.1f, 8, CYBER_PANEL);
    DrawRectangleRoundedLinesEx({px, py, 340, 240}, 0.1f, 8, 2.0f, CYBER_CYAN);
    DrawText("OPERATOR: KAEL", (int)px + 20, (int)py + 20, 20, CYBER_CYAN);
    
    if (player_) {
        // Player HP Bar
        DrawText(TextFormat("HEALTH: %d / 100", player_->getHealth()), (int)px + 20, (int)py + 65, 14, CYBER_TEXT);
        DrawRectangleRounded({px + 20, py + 88, 300, 16}, 0.5f, 4, CYBER_DARK);
        DrawRectangleRounded({px + 20, py + 88, 300.0f * player_->getHealthPercent(), 16}, 0.5f, 4, CYBER_SUCCESS);
        DrawRectangleRoundedLinesEx({px + 20, py + 88, 300, 16}, 0.5f, 4, 1.5f, CYBER_TEXT);

        // Player Energy Bar
        DrawText(TextFormat("CORE CAPACITY: %d / 100", player_->getEnergy()), (int)px + 20, (int)py + 120, 14, CYBER_TEXT);
        DrawRectangleRounded({px + 20, py + 142, 300, 14}, 0.5f, 4, CYBER_DARK);
        DrawRectangleRounded({px + 20, py + 142, 300.0f * player_->getEnergyPercent(), 14}, 0.5f, 4, CYBER_CYAN);
    }
    DrawText("EQUIPPED: ARRAY CORE [O(1) ACCESS]", (int)px + 20, (int)py + 185, 14, CYBER_WARNING);

    // ─── ENEMY STATUS CARD (Right) ───
    float ex = sw - 440;
    float ey = 160;
    DrawRectangleRounded({ex, ey, 340, 240}, 0.1f, 8, CYBER_PANEL);
    DrawRectangleRoundedLinesEx({ex, ey, 340, 240}, 0.1f, 8, 2.0f, CYBER_DANGER);
    DrawText("TARGET: MEMORY EATER", (int)ex + 20, (int)ey + 20, 20, CYBER_DANGER);

    if (enemy_) {
        // Enemy HP Bar
        DrawText(TextFormat("CORRUPTION INTEGRITY: %d / 100", enemy_->getHealth()), (int)ex + 20, (int)ey + 65, 14, CYBER_TEXT);
        DrawRectangleRounded({ex + 20, py + 88, 300, 16}, 0.5f, 4, CYBER_DARK);
        DrawRectangleRounded({ex + 20, py + 88, 300.0f * enemy_->getHealthPercent(), 16}, 0.5f, 4, CYBER_DANGER);
        DrawRectangleRoundedLinesEx({ex + 20, py + 88, 300, 16}, 0.5f, 4, 1.5f, RED);

        DrawText("WEAKNESS: DIRECT INDEX SEARCH (INDEX 0)", (int)ex + 20, (int)ey + 130, 13, CYBER_TEXT);
        DrawText("VULNERABILITY: UNPROTECTED BUFFER ADDRESS", (int)ex + 20, (int)ey + 160, 13, CYBER_DIM);
    }

    // ─── CENTER ARENA VISUAL ───
    float cx = sw / 2.0f;
    float cy = 280.0f;
    // Energy clash circle
    float pulse = (sinf(time * 5.0f) + 1.0f) * 0.5f;
    DrawCircleLines((int)cx, (int)cy, 50.0f + pulse * 10.0f, CYBER_CYAN);
    DrawText("VS", (int)cx - 15, (int)cy - 12, 24, CYBER_TEXT);

    // ─── COMBAT ACTION DOCK (Bottom) ───
    float dockW = 860.0f;
    float dockH = 150.0f;
    float dockX = sw/2.0f - dockW/2.0f;
    float dockY = sh - dockH - 40.0f;

    DrawRectangleRounded({dockX, dockY, dockW, dockH}, 0.1f, 8, CYBER_PANEL);
    DrawRectangleRoundedLinesEx({dockX, dockY, dockW, dockH}, 0.1f, 8, 2.0f, CYBER_CYAN);

    if (state_ == CombatState::PLAYER_TURN) {
        DrawText("OPERATOR TURN — EXECUTE ALGORITHM ACTION:", (int)dockX + 30, (int)dockY + 20, 18, CYBER_CYAN);

        // Action [1]
        DrawRectangleRounded({dockX + 30, dockY + 55, 380, 65}, 0.15f, 6, Color{20, 32, 50, 255});
        DrawRectangleRoundedLinesEx({dockX + 30, dockY + 55, 380, 65}, 0.15f, 6, 2.0f, CYBER_CYAN);
        DrawText("[1] ARRAY ACCESS SCAN (50 DMG)", (int)dockX + 45, (int)dockY + 70, 16, CYBER_TEXT);
        DrawText("Direct memory dereference at index 0 — O(1)", (int)dockX + 45, (int)dockY + 92, 12, CYBER_DIM);

        // Action [2]
        DrawRectangleRounded({dockX + 450, dockY + 55, 380, 65}, 0.15f, 6, Color{20, 32, 50, 255});
        DrawRectangleRoundedLinesEx({dockX + 450, dockY + 55, 380, 65}, 0.15f, 6, 2.0f, CYBER_DIM);
        DrawText("[SPACE] LINEAR STRIKE (25 DMG)", (int)dockX + 465, (int)dockY + 70, 16, CYBER_TEXT);
        DrawText("Sequential memory traverse — O(n)", (int)dockX + 465, (int)dockY + 92, 12, CYBER_DIM);

    } else if (state_ == CombatState::ENEMY_TURN) {
        DrawText("ENEMY PROCESSING TURN: Anomaly corrupting local cache...", (int)dockX + 30, (int)dockY + 60, 20, CYBER_DANGER);
    } else if (state_ == CombatState::VICTORY) {
        DrawText("TARGET NEUTRALIZED! MEMORY SECTOR SECURED.", (int)dockX + 30, (int)dockY + 60, 22, CYBER_SUCCESS);
    } else if (state_ == CombatState::DEFEAT) {
        DrawText("SYSTEM CRITICAL: OPERATOR OVERRIDDEN.", (int)dockX + 30, (int)dockY + 60, 22, CYBER_DANGER);
    }
}

void CombatSystem::playerAttack(int damage) {
    if (state_ == CombatState::PLAYER_TURN && enemy_) {
        enemy_->takeDamage(damage);
        state_ = CombatState::ENEMY_TURN;
        turnTimer_ = 0.0f;
    }
}

void CombatSystem::triggerDSAPuzzle() {
    state_ = CombatState::PUZZLE_PHASE;
}

void CombatSystem::endCombat(bool playerWon) {
    LOG_INFO(playerWon ? "Combat won!" : "Combat lost.");
    state_ = CombatState::INACTIVE;
    player_ = nullptr;
    enemy_ = nullptr;
}

bool CombatSystem::isActive() const { return state_ != CombatState::INACTIVE; }
CombatState CombatSystem::getState() const { return state_; }
