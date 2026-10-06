#pragma once
#include <string>
#include "raylib.h"

class HUD {
private:
    int health_ = 100;
    int maxHealth_ = 100;
    float displayHealth_ = 100.0f;
    
    int energy_ = 100;
    int maxEnergy_ = 100;
    float displayEnergy_ = 100.0f;
    
    std::string abilityName_ = "NONE";
    std::string objective_ = "Find the exit";
    std::string chapter_ = "CHAPTER 1 - MEMORY DISTRICT";
    bool showPrompt_ = false;

public:
    void update(float dt);
    void draw();
    
    void setHealth(int h, int m);
    void setEnergy(int e, int m);
    void setAbility(const std::string& name);
    void setObjective(const std::string& obj);
    void setChapter(const std::string& chap);
    void showInteractionPrompt(bool show);
};
