#include "Enemy.h"
#include "ui/UISystem.h"
#include <cmath>

static Texture2D s_eaterTexture = {0};
static bool s_eaterTextureLoaded = false;

Enemy::Enemy(std::string id, std::string name, float x, float y, int hp, int damage, std::string weakness)
    : Character(id, name, x, y, hp, 100), damage_(damage), detectionRange_(150.0f), attackCooldown_(2.0f), attackTimer_(0.0f),
      state_(EnemyState::PATROL), dsaWeakness_(weakness), stunTimer_(0.0f), patrolTimer_(0.0f), patrolDir_(1) 
{
    bounds_ = {x, y, 36.0f, 44.0f};
    if (!s_eaterTextureLoaded && IsWindowReady()) {
        if (FileExists("assets/enemies/memory_eater.png")) {
            s_eaterTexture = LoadTexture("assets/enemies/memory_eater.png");
            s_eaterTextureLoaded = true;
        }
    }
}

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
        if (patrolTimer_ > 2.2f) {
            patrolDir_ *= -1;
            patrolTimer_ = 0.0f;
        }
        velocity_.x = patrolDir_ * 65.0f;
    }

    Character::update(dt);
}

void Enemy::draw() {
    if (state_ == EnemyState::DEAD) return;

    float time = (float)GetTime();
    float glitchOffset = (state_ == EnemyState::STUNNED) ? (sinf(time * 30.0f) * 3.0f) : (sinf(time * 8.0f) * 1.5f);

    // Corrupted anomaly ground shadow & red hazard pulse
    DrawEllipse((int)(position_.x + 18), (int)(position_.y + 42), 16, 6, Color{0, 0, 0, 110});
    DrawCircleGradient((int)(position_.x + 18), (int)(position_.y + 22), 24.0f, Color{255, 60, 80, 60}, Color{255, 60, 80, 0});

    if (s_eaterTextureLoaded && s_eaterTexture.id > 0) {
        Color tint = (state_ == EnemyState::STUNNED) ? Color{200, 100, 255, 220} : WHITE;
        DrawTexture(s_eaterTexture, (int)(position_.x + glitchOffset), (int)position_.y, tint);
    } else {
        Color c = (state_ == EnemyState::STUNNED) ? CYBER_MAGENTA : CYBER_DANGER;
        DrawRectangleRounded({position_.x + glitchOffset, position_.y, 36.0f, 44.0f}, 0.2f, 6, c);
        DrawRectangleRoundedLinesEx({position_.x + glitchOffset, position_.y, 36.0f, 44.0f}, 0.2f, 6, 2.0f, RED);
    }

    // Overhead HP bar & name
    int barW = 44;
    int barH = 5;
    DrawRectangle((int)(position_.x + 18 - barW/2), (int)(position_.y - 12), barW, barH, Color{20, 20, 30, 220});
    DrawRectangle((int)(position_.x + 18 - barW/2), (int)(position_.y - 12), (int)(barW * getHealthPercent()), barH, CYBER_DANGER);
    DrawRectangleLines((int)(position_.x + 18 - barW/2), (int)(position_.y - 12), barW, barH, Color{100, 110, 140, 180});

    int nw = MeasureText(name_.c_str(), 10);
    DrawText(name_.c_str(), (int)(position_.x + 18 - nw/2), (int)(position_.y - 24), 10, CYBER_DANGER);
}

std::string Enemy::getType() const { return "Enemy"; }
std::string Enemy::getDSAWeakness() const { return dsaWeakness_; }
int Enemy::getDamage() const { return damage_; }
void Enemy::stun(float duration) {
    state_ = EnemyState::STUNNED;
    stunTimer_ = duration;
}
