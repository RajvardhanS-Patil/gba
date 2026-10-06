#pragma once
#include "Entity.h"
#include <string>
#include <algorithm>
#include <raylib.h>

enum class AnimState { IDLE, WALKING, INTERACTING, COMBAT, HIT, VICTORY };

class Character : public Entity {
public:
    Character(std::string id, std::string name, float x, float y, int maxHealth, int maxEnergy);
    virtual ~Character() = default;

    virtual void takeDamage(int amount);
    virtual void heal(int amount);
    virtual void useEnergy(int amount);

    bool isAlive() const;
    int getHealth() const { return health_; }
    int getEnergy() const { return energy_; }
    float getHealthPercent() const;
    float getEnergyPercent() const;

    void setAnimState(AnimState state);
    
    void update(float dt) override;
    void draw() override;
    std::string getType() const override;

protected:
    std::string name_;
    int health_;
    int maxHealth_;
    int energy_;
    int maxEnergy_;
    AnimState animState_;
    float animTimer_;
    int animFrame_;
};
