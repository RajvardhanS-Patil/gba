#include "DSAVisualizer.h"
#include "raylib.h"

extern const Color CYBER_CYAN;
extern const Color CYBER_PANEL;
extern const Color CYBER_TEXT;
extern const Color CYBER_DIM;

DSAVisualizer::DSAVisualizer() {
    buttons_.push_back(std::make_unique<UISystem::Button>((Rectangle){0,0,100,30}, "INSERT", [](){}));
    buttons_.push_back(std::make_unique<UISystem::Button>((Rectangle){0,0,100,30}, "REMOVE", [](){}));
    buttons_.push_back(std::make_unique<UISystem::Button>((Rectangle){0,0,100,30}, "SEARCH", [](){}));
    buttons_.push_back(std::make_unique<UISystem::Button>((Rectangle){0,0,100,30}, "RESET", [](){}));
}

void DSAVisualizer::addLog(const std::string& msg) {
    log_.push_back(msg);
    if (log_.size() > 5) log_.erase(log_.begin());
}

void DSAVisualizer::update(IDataStructure* ds) {
    (void)ds;
    for (auto& btn : buttons_) btn->update();
}

void DSAVisualizer::draw(IDataStructure* ds, float x, float y, float w, float h) {
    DrawRectangleRounded({x, y, w, h}, 0.05f, 8, CYBER_PANEL);
    DrawRectangleRoundedLinesEx({x, y, w, h}, 0.05f, 8, 2.0f, CYBER_CYAN);
    
    if (ds) {
        DrawText(ds->getName().c_str(), static_cast<int>(x + 20), static_cast<int>(y + 20), 30, CYBER_CYAN);
        DrawText(ds->getComplexityInfo().c_str(), static_cast<int>(x + 20), static_cast<int>(y + 60), 15, CYBER_DIM);
        
        ds->visualize(x + 20, y + 100, w - 40, h - 200);
    } else {
        DrawText("NO DATA STRUCTURE LOADED", static_cast<int>(x + w/2 - 150), static_cast<int>(y + h/2), 20, CYBER_DIM);
    }
    
    // Draw Controls
    for (size_t i = 0; i < buttons_.size(); i++) {
        buttons_[i]->setPosition(x + 20 + i*120, y + h - 50);
        buttons_[i]->draw();
    }
    
    // Draw Log
    float logY = y + h - 150;
    DrawText("OPERATIONS LOG:", x + w - 300, logY, 15, CYBER_CYAN);
    for (int i = 0; i < log_.size(); i++) {
        DrawText(log_[i].c_str(), x + w - 300, logY + 25 + i*20, 15, CYBER_TEXT);
    }
}
