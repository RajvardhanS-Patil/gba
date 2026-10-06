#include "NPC.h"
#include "../utils/MathUtils.h"

NPC::NPC(std::string id, std::string name, float x, float y, std::string dialogueId)
    : Character(id, name, x, y, 100, 100), dialogueId_(dialogueId), interactionRadius_(60.0f), hasInteracted_(false) {}

void NPC::update(float dt) {
    setAnimState(AnimState::IDLE);
    Character::update(dt);
}

void NPC::draw() {
    DrawRectangleRec(bounds_, VIOLET);
    DrawText(name_.c_str(), position_.x, position_.y - 10, 10, WHITE);
}

std::string NPC::getType() const { return "NPC"; }

std::string NPC::getDialogueId() const { return dialogueId_; }

bool NPC::isInRange(Vector2 playerPos) const {
    Vector2 center = {position_.x + bounds_.width/2, position_.y + bounds_.height/2};
    float dist = MathUtils::distance(center, playerPos);
    return dist <= interactionRadius_;
}
