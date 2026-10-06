#pragma once
#include "../entities/Player.h"
#include "../entities/Enemy.h"

enum class CombatState { INACTIVE, PLAYER_TURN, ENEMY_TURN, PUZZLE_PHASE, VICTORY, DEFEAT };

class CombatSystem {
public:
    CombatSystem();
    ~CombatSystem() = default;

    void startCombat(Player* player, Enemy* enemy);
    void update(float dt);
    void draw();

    void playerAttack(int damage);
    void triggerDSAPuzzle();
    void endCombat(bool playerWon);

    bool isActive() const;
    CombatState getState() const;

private:
    CombatState state_;
    Player* player_;
    Enemy* enemy_;
    float turnTimer_;
};
