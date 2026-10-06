#pragma once
#include <string>
#include <vector>
#include "raylib.h"

class DialogueUI {
private:
    float typeTimer_ = 0.0f;
    int charIndex_ = 0;
    int selectedChoice_ = 0;
    
public:
    void draw(const std::string& speaker, const std::string& text, bool& isComplete, const std::vector<std::string>& choices);
    void update(float dt, const std::string& text, bool& isComplete, const std::vector<std::string>& choices, int& selectedOut);
};
