#pragma once
#include "Character.h"

enum class EnemyState { PATROL, CHASE, ATTACK, STUNNED, DEAD };

class Enemy : public Character {
public:
    Enemy(std::string id, std::string name, float x, float y, int hp, int damage, std::string weakness);
    virtual ~Enemy() = default;

    void update(float dt) override;
    void draw() override;
    std::string getType() const override;

    std::string getDSAWeakness() const;
    int getDamage() const;
    void stun(float duration);

private:
    int damage_;
    float detectionRange_;
    float attackCooldown_;
    float attackTimer_;
    EnemyState state_;
    std::string dsaWeakness_;
    float stunTimer_;
    float patrolTimer_;
    int patrolDir_;
};
