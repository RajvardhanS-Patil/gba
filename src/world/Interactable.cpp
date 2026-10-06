#include "Interactable.h"
#include "../utils/MathUtils.h"
#include "../utils/Logger.h"

Interactable::Interactable(std::string id, std::string type, float x, float y, float w, float h)
    : id_(id), position_{x, y}, bounds_{x, y, w, h}, type_(type), activated_(false), linkedAction_("") {}

void Interactable::interact() {
    activated_ = !activated_;
    LOG_INFO("Interacted with " + id_);
}

void Interactable::draw() {
    Color c = activated_ ? GREEN : YELLOW;
    DrawRectangleLinesEx(bounds_, 2.0f, c);
    DrawText(id_.c_str(), position_.x, position_.y - 15, 10, c);
}

bool Interactable::isActivated() const { return activated_; }
std::string Interactable::getLinkedAction() const { return linkedAction_; }

bool Interactable::isInRange(Vector2 playerPos, float range) const {
    Vector2 center = {position_.x + bounds_.width/2, position_.y + bounds_.height/2};
    return MathUtils::distance(center, playerPos) <= range;
}
