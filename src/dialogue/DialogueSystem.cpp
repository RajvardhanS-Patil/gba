#include "DialogueSystem.h"
#include <fstream>
#include "../utils/Logger.h"
#include <raylib.h>

DialogueSystem::DialogueSystem() : currentNode_(nullptr), active_(false), typewriterProgress_(0.0f), typewriterSpeed_(30.0f), waitingForInput_(false), selectedChoice_(0) {}

void DialogueSystem::loadDialogue(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        LOG_ERROR("Failed to load dialogue: " + filepath);
        return;
    }
    try {
        nlohmann::json j;
        file >> j;
        if (j.contains("nodes")) {
            for (const auto& nData : j["nodes"]) {
                DialogueNode node = DialogueNode::fromJson(nData);
                nodes_[node.id] = node;
            }
        }
        LOG_INFO("Dialogue loaded from " + filepath);
    } catch (const std::exception& e) {
        LOG_ERROR("Failed to parse dialogue JSON: " + std::string(e.what()));
    }
}

void DialogueSystem::startDialogue(const std::string& nodeId) {
    auto it = nodes_.find(nodeId);
    if (it != nodes_.end()) {
        currentNode_ = &(it->second);
        active_ = true;
        typewriterProgress_ = 0.0f;
        displayedText_ = "";
        waitingForInput_ = true;
        selectedChoice_ = 0;
    }
}

void DialogueSystem::update(float dt) {
    if (!active_ || !currentNode_) return;

    if (typewriterProgress_ < currentNode_->text.length()) {
        typewriterProgress_ += dt * typewriterSpeed_;
        if (typewriterProgress_ > currentNode_->text.length()) {
            typewriterProgress_ = currentNode_->text.length();
        }
        displayedText_ = currentNode_->text.substr(0, static_cast<size_t>(typewriterProgress_));
    }

    if (isTypewriterComplete()) {
        if (!currentNode_->choices.empty()) {
            if (IsKeyPressed(KEY_UP)) selectedChoice_ = std::max(0, selectedChoice_ - 1);
            if (IsKeyPressed(KEY_DOWN)) selectedChoice_ = std::min(static_cast<int>(currentNode_->choices.size() - 1), selectedChoice_ + 1);
            if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) selectChoice(selectedChoice_);
        } else {
            if (IsKeyPressed(KEY_SPACE)) advanceDialogue();
        }
    } else {
        if (IsKeyPressed(KEY_SPACE)) skipTypewriter();
    }
}

DialogueNode* DialogueSystem::getCurrentNode() { return currentNode_; }
std::string DialogueSystem::getDisplayedText() const { return displayedText_; }
bool DialogueSystem::isActive() const { return active_; }
bool DialogueSystem::isTypewriterComplete() const {
    if (!currentNode_) return false;
    return typewriterProgress_ >= currentNode_->text.length();
}

void DialogueSystem::advanceDialogue() {
    if (currentNode_ && !currentNode_->nextNodeId.empty()) {
        startDialogue(currentNode_->nextNodeId);
    } else {
        active_ = false;
        currentNode_ = nullptr;
    }
}

void DialogueSystem::selectChoice(int index) {
    if (currentNode_ && index >= 0 && index < currentNode_->choices.size()) {
        std::string nextId = currentNode_->choices[index].nextNodeId;
        if (!nextId.empty()) {
            startDialogue(nextId);
        } else {
            active_ = false;
            currentNode_ = nullptr;
        }
    }
}

void DialogueSystem::skipTypewriter() {
    if (currentNode_) {
        typewriterProgress_ = currentNode_->text.length();
        displayedText_ = currentNode_->text;
    }
}
