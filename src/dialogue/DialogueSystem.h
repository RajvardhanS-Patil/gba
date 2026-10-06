#pragma once
#include <string>
#include <map>
#include "DialogueNode.h"

class DialogueSystem {
public:
    DialogueSystem();
    ~DialogueSystem() = default;

    void loadDialogue(const std::string& filepath);
    void startDialogue(const std::string& nodeId);
    void update(float dt);
    
    DialogueNode* getCurrentNode();
    std::string getDisplayedText() const;
    
    bool isActive() const;
    bool isTypewriterComplete() const;
    
    void advanceDialogue();
    void selectChoice(int index);
    void skipTypewriter();

private:
    std::map<std::string, DialogueNode> nodes_;
    DialogueNode* currentNode_;
    bool active_;
    
    float typewriterProgress_;
    float typewriterSpeed_;
    std::string displayedText_;
    
    bool waitingForInput_;
    int selectedChoice_;
};
