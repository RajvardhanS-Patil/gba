#include "CombatSystem.h"
#include <raylib.h>
#include "../utils/Logger.h"

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

    if (!player_->isAlive()) {
        state_ = CombatState::DEFEAT;
        turnTimer_ += dt;
        if (turnTimer_ > 2.0f) endCombat(false);
        return;
    }

    if (!enemy_->isAlive()) {
        state_ = CombatState::VICTORY;
        turnTimer_ += dt;
        if (turnTimer_ > 2.0f) endCombat(true);
        return;
    }

    if (state_ == CombatState::ENEMY_TURN) {
        turnTimer_ += dt;
        if (turnTimer_ > 1.5f) {
            player_->takeDamage(enemy_->getDamage());
            state_ = CombatState::PLAYER_TURN;
            turnTimer_ = 0.0f;
        }
    }
}

void CombatSystem::draw() {
    if (state_ == CombatState::INACTIVE) return;

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
    DrawText("COMBAT ACTIVE", 20, 20, 20, RED);

    if (player_) {
        std::string hp = "Player HP: " + std::to_string(static_cast<int>(player_->getHealthPercent() * 100)) + "%";
        DrawText(hp.c_str(), 20, 50, 20, GREEN);
    }
    if (enemy_) {
        std::string hp = "Enemy HP: " + std::to_string(static_cast<int>(enemy_->getHealthPercent() * 100)) + "%";
        DrawText(hp.c_str(), GetScreenWidth() - 200, 50, 20, RED);
    }
    
    if (state_ == CombatState::PLAYER_TURN) {
        DrawText("Your Turn! (Press Space to Attack)", 20, GetScreenHeight() - 50, 20, WHITE);
        if (IsKeyPressed(KEY_SPACE)) playerAttack(25);
    } else if (state_ == CombatState::ENEMY_TURN) {
        DrawText("Enemy Turn...", 20, GetScreenHeight() - 50, 20, GRAY);
    } else if (state_ == CombatState::VICTORY) {
        DrawText("VICTORY!", GetScreenWidth()/2 - 50, GetScreenHeight()/2, 30, YELLOW);
    } else if (state_ == CombatState::DEFEAT) {
        DrawText("DEFEAT...", GetScreenWidth()/2 - 50, GetScreenHeight()/2, 30, RED);
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
