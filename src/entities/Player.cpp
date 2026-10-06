#include "Player.h"
#include "ui/UISystem.h"

Player::Player(float x, float y) : Character("player", "Player", x, y, 100, 100), moveSpeed_(200.0f), equippedAbility_(-1), canInteract_(false) {}

void Player::update(float dt) {
    handleInput(dt);
    Character::update(dt);
}

void Player::handleInput(float dt) {
    (void)dt;
    velocity_ = {0, 0};
    if (IsKeyDown(KEY_W)) velocity_.y -= moveSpeed_;
    if (IsKeyDown(KEY_S)) velocity_.y += moveSpeed_;
    if (IsKeyDown(KEY_A)) velocity_.x -= moveSpeed_;
    if (IsKeyDown(KEY_D)) velocity_.x += moveSpeed_;
    
    if (velocity_.x != 0 || velocity_.y != 0) {
        setAnimState(AnimState::WALKING);
    } else {
        setAnimState(AnimState::IDLE);
    }
}

void Player::draw() {
    DrawRectangleRec(bounds_, CYBER_CYAN);
    DrawRectangleLinesEx(bounds_, 2.0f, BLUE);
    DrawText(name_.c_str(), static_cast<int>(position_.x), static_cast<int>(position_.y - 10), 10, WHITE);
}

std::string Player::getType() const {
    return "Player";
}

void Player::setCanInteract(bool can) { canInteract_ = can; }
bool Player::getCanInteract() const { return canInteract_; }

Rectangle Player::getInteractionRect() const {
    return {position_.x - 10, position_.y - 10, bounds_.width + 20, bounds_.height + 20};
}
