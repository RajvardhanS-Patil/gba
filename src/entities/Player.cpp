#include "Player.h"
#include "audio/SFXManager.h"
#include <cmath>

Player::Player(int tileX, int tileY)
    : Character("kael", "Kael", static_cast<float>(tileX * 16), static_cast<float>(tileY * 16 - 4), 100, 100),
      currentTileX_(tileX), currentTileY_(tileY), targetTileX_(tileX), targetTileY_(tileY) 
{
    if (FileExists("assets/sprites/kael_walk.png")) {
        spriteSheet_ = LoadTexture("assets/sprites/kael_walk.png");
        spriteLoaded_ = true;
    }
}

Player::~Player() {
    if (spriteLoaded_ && spriteSheet_.id > 0) {
        UnloadTexture(spriteSheet_);
        spriteLoaded_ = false;
    }
}

void Player::setTilePosition(int tx, int ty) {
    currentTileX_ = tx;
    currentTileY_ = ty;
    targetTileX_ = tx;
    targetTileY_ = ty;
    isMoving_ = false;
    moveTimer_ = 0.0f;
    position_ = {static_cast<float>(tx * 16), static_cast<float>(ty * 16 - 4)};
}

void Player::handleInput(const Tilemap* map, float dt, std::function<bool(int, int)> extraSolidCheck) {
    if (isMoving_) return;

    int dx = 0;
    int dy = 0;

    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        dy = -1;
        facing_ = Facing::UP;
    } else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        dy = 1;
        facing_ = Facing::DOWN;
    } else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        dx = -1;
        facing_ = Facing::LEFT;
    } else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        dx = 1;
        facing_ = Facing::RIGHT;
    }

    if (dx != 0 || dy != 0) {
        int nextX = currentTileX_ + dx;
        int nextY = currentTileY_ + dy;

        bool blocked = (map && map->isSolid(nextX, nextY));
        if (!blocked && extraSolidCheck && extraSolidCheck(nextX, nextY)) {
            blocked = true;
        }

        if (blocked) {
            // Collision bump
            SFXManager::instance().play("hover"); // soft bump sound
        } else {
            targetTileX_ = nextX;
            targetTileY_ = nextY;
            isMoving_ = true;
            moveTimer_ = 0.0f;
        }
    }
}

void Player::update(float dt) {
    if (isMoving_) {
        moveTimer_ += dt;
        float progress = moveTimer_ / moveDuration_;
        if (progress >= 1.0f) {
            currentTileX_ = targetTileX_;
            currentTileY_ = targetTileY_;
            isMoving_ = false;
            moveTimer_ = 0.0f;
            position_ = {static_cast<float>(currentTileX_ * 16), static_cast<float>(currentTileY_ * 16 - 4)};
        } else {
            float startX = static_cast<float>(currentTileX_ * 16);
            float startY = static_cast<float>(currentTileY_ * 16 - 4);
            float endX   = static_cast<float>(targetTileX_ * 16);
            float endY   = static_cast<float>(targetTileY_ * 16 - 4);

            position_.x = startX + (endX - startX) * progress;
            position_.y = startY + (endY - startY) * progress;
        }

        // Step animation
        animTimer_ += dt;
        if (animTimer_ >= 0.11f) {
            animTimer_ = 0.0f;
            animFrame_ = (animFrame_ + 1) % 4; // 0, 1, 2, 1
        }
    } else {
        animFrame_ = 0; // Idle frame
        animTimer_ = 0.0f;
    }
}

void Player::draw() {
    int col = 0;
    if (animFrame_ == 1) col = 1;
    else if (animFrame_ == 3) col = 2;
    int row = static_cast<int>(facing_);

    if (spriteLoaded_ && spriteSheet_.id > 0) {
        Rectangle srcRec = {
            static_cast<float>(col * 16),
            static_cast<float>(row * 20),
            16.0f,
            20.0f
        };
        Vector2 drawPos = {position_.x, position_.y};
        DrawTextureRec(spriteSheet_, srcRec, drawPos, WHITE);
    } else {
        // Fallback pixel rect
        DrawRectangle(static_cast<int>(position_.x), static_cast<int>(position_.y + 4), 16, 16, RED);
    }
}

void Player::getFacingTile(int& outX, int& outY) const {
    outX = currentTileX_;
    outY = currentTileY_;
    switch (facing_) {
        case Facing::DOWN:  outY += 1; break;
        case Facing::UP:    outY -= 1; break;
        case Facing::LEFT:  outX -= 1; break;
        case Facing::RIGHT: outX += 1; break;
    }
}
