#include "Character.h"

Character::Character(std::string id, std::string name, float x, float y, int maxHealth, int maxEnergy)
    : Entity(id, x, y), name_(name), health_(maxHealth), maxHealth_(maxHealth), energy_(maxEnergy), maxEnergy_(maxEnergy),
      animState_(AnimState::IDLE), animTimer_(0.0f), animFrame_(0) {}

void Character::takeDamage(int amount) {
    health_ -= amount;
    if (health_ < 0) health_ = 0;
}

void Character::heal(int amount) {
    health_ += amount;
    if (health_ > maxHealth_) health_ = maxHealth_;
}

void Character::useEnergy(int amount) {
    energy_ -= amount;
    if (energy_ < 0) energy_ = 0;
}

bool Character::isAlive() const {
    return health_ > 0;
}

float Character::getHealthPercent() const {
    return static_cast<float>(health_) / maxHealth_;
}

float Character::getEnergyPercent() const {
    return static_cast<float>(energy_) / maxEnergy_;
}

void Character::setAnimState(AnimState state) {
    animState_ = state;
}

void Character::update(float dt) {
    position_.x += velocity_.x * dt;
    position_.y += velocity_.y * dt;
    bounds_.x = position_.x;
    bounds_.y = position_.y;
}

void Character::draw() {
    DrawRectangleRec(bounds_, GRAY);
    DrawText(name_.c_str(), position_.x, position_.y - 10, 10, WHITE);
}

std::string Character::getType() const {
    return "Character";
}
