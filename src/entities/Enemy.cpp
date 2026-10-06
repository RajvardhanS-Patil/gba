#include "Enemy.h"

Enemy::Enemy(std::string id, std::string name, float x, float y, int hp, int damage, std::string weakness)
    : Character(id, name, x, y, hp, 100), damage_(damage), detectionRange_(150.0f), attackCooldown_(2.0f), attackTimer_(0.0f),
      state_(EnemyState::PATROL), dsaWeakness_(weakness), stunTimer_(0.0f), patrolTimer_(0.0f), patrolDir_(1) {}

void Enemy::update(float dt) {
    if (!isAlive()) {
        state_ = EnemyState::DEAD;
        return;
    }

    if (state_ == EnemyState::STUNNED) {
        stunTimer_ -= dt;
        if (stunTimer_ <= 0) state_ = EnemyState::PATROL;
        velocity_ = {0, 0};
    } else if (state_ == EnemyState::PATROL) {
        patrolTimer_ += dt;
        if (patrolTimer_ > 2.0f) {
            patrolDir_ *= -1;
            patrolTimer_ = 0.0f;
        }
        velocity_.x = patrolDir_ * 50.0f;
    }

    Character::update(dt);
}

void Enemy::draw() {
    if (state_ == EnemyState::DEAD) return;
    Color c = (state_ == EnemyState::STUNNED) ? MAGENTA : RED;
    DrawRectangleRec(bounds_, c);
    DrawText(name_.c_str(), position_.x, position_.y - 10, 10, WHITE);
}

std::string Enemy::getType() const { return "Enemy"; }
std::string Enemy::getDSAWeakness() const { return dsaWeakness_; }
int Enemy::getDamage() const { return damage_; }
void Enemy::stun(float duration) {
    state_ = EnemyState::STUNNED;
    stunTimer_ = duration;
}
