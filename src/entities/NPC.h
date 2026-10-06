#pragma once
#include "Character.h"

class NPC : public Character {
public:
    NPC(std::string id, std::string name, float x, float y, std::string dialogueId);
    virtual ~NPC() = default;

    void update(float dt) override;
    void draw() override;
    std::string getType() const override;

    std::string getDialogueId() const;
    bool isInRange(Vector2 playerPos) const;

private:
    std::string dialogueId_;
    float interactionRadius_;
    bool hasInteracted_;
};
