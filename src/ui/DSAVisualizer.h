#pragma once
#include "../dsa/IDataStructure.h"
#include <memory>
#include <vector>
#include "UISystem.h"

class DSAVisualizer {
private:
    std::vector<std::unique_ptr<UISystem::Button>> buttons_;
    std::vector<std::string> log_;
    
public:
    DSAVisualizer();
    void update(IDataStructure* ds);
    void draw(IDataStructure* ds, float x, float y, float w, float h);
    void addLog(const std::string& msg);
};
