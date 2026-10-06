#pragma once
#include "Character.h"

class Player : public Character {
public:
    Player(float x, float y);
    virtual ~Player() = default;

    void update(float dt) override;
    void draw() override;
    std::string getType() const override;

    void handleInput(float dt);
    
    void setCanInteract(bool can);
    bool getCanInteract() const;
    Rectangle getInteractionRect() const;

private:
    float moveSpeed_;
    int equippedAbility_;
    bool canInteract_;
};
