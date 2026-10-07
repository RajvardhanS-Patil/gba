#pragma once
#include "Character.h"
#include "world/Tilemap.h"

enum class Facing { DOWN = 0, UP = 1, LEFT = 2, RIGHT = 3 };

class Player : public Character {
public:
    Player(int tileX, int tileY);
    ~Player() override;

    void update(float dt) override;
    void draw() override;
    std::string getType() const override { return "Player"; }

#include <functional>

    void handleInput(const Tilemap* map, float dt, std::function<bool(int, int)> extraSolidCheck = nullptr);
    
    int getTileX() const { return currentTileX_; }
    int getTileY() const { return currentTileY_; }
    void setTilePosition(int tx, int ty);

    Facing getFacing() const { return facing_; }
    bool isMoving() const { return isMoving_; }

    // Interacting tile in front of Kael
    void getFacingTile(int& outX, int& outY) const;

private:
    int currentTileX_ = 0;
    int currentTileY_ = 0;
    int targetTileX_ = 0;
    int targetTileY_ = 0;

    Facing facing_ = Facing::DOWN;
    bool isMoving_ = false;
    float moveTimer_ = 0.0f;
    float moveDuration_ = 0.22f; // Smooth 16px tile glide

    int animFrame_ = 0;
    float animTimer_ = 0.0f;

    Texture2D spriteSheet_{0};
    bool spriteLoaded_ = false;
};
