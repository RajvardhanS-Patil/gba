#include "Stack.h"
#include "raylib.h"
#include <string>

extern const Color CYBER_CYAN;
extern const Color CYBER_PANEL;
extern const Color CYBER_TEXT;
extern const Color CYBER_DIM;

void Stack::insert(int value) { push(value); }

void Stack::push(int value) {
    if (!isFull()) {
        data_.push_back(value);
        animProgress_ = 0.0f;
        animatingValue_ = value;
        pushing_ = true;
        popping_ = false;
    }
}

int Stack::pop() {
    if (!empty()) {
        int val = data_.back();
        data_.pop_back();
        animProgress_ = 0.0f;
        animatingValue_ = val;
        pushing_ = false;
        popping_ = true;
        return val;
    }
    return -1;
}

int Stack::peek() const { return empty() ? -1 : data_.back(); }
bool Stack::isFull() const { return data_.size() >= maxSize_; }

bool Stack::remove(int value) {
    if (!empty() && data_.back() == value) { pop(); return true; }
    return false;
}

bool Stack::search(int value) const {
    for (int v : data_) if (v == value) return true;
    return false;
}

std::string Stack::getName() const { return "Stack"; }
std::string Stack::getComplexityInfo() const { return "Push: O(1) | Pop: O(1) | Peek: O(1)"; }
size_t Stack::size() const { return data_.size(); }
bool Stack::empty() const { return data_.empty(); }
void Stack::reset() { data_.clear(); pushing_ = popping_ = false; animProgress_ = 1.0f; }

void Stack::updateAnim(float dt) {
    if (animProgress_ < 1.0f) {
        animProgress_ += dt * 5.0f;
        if (animProgress_ > 1.0f) {
            animProgress_ = 1.0f;
            pushing_ = popping_ = false;
        }
    }
}

void Stack::visualize(float x, float y, float w, float h) const {
    float elemW = 120;
    float elemH = 40;
    float spacing = 5;
    float startX = x + w/2 - elemW/2;
    float bottomY = y + h - 50;

    DrawText("BOTTOM", startX + elemW/2 - MeasureText("BOTTOM", 10)/2, bottomY + 10, 10, CYBER_DIM);

    for (size_t i = 0; i < data_.size(); i++) {
        float ey = bottomY - (i + 1) * (elemH + spacing);
        
        bool isTop = (i == data_.size() - 1);
        Color borderColor = isTop ? CYBER_CYAN : CYBER_DIM;
        
        if (isTop && pushing_) {
            ey -= (1.0f - animProgress_) * 50.0f; 
        }

        if (isTop && !pushing_ && !popping_) {
            DrawRectangleRounded({startX-2, ey-2, elemW+4, elemH+4}, 0.2f, 8, {0, 229, 255, 100});
            DrawText("TOP ->", static_cast<int>(startX - 60), static_cast<int>(ey + elemH/2 - 5), 10, CYBER_CYAN);
        }

        DrawRectangleRounded({startX, ey, elemW, elemH}, 0.2f, 8, CYBER_PANEL);
        DrawRectangleRoundedLinesEx({startX, ey, elemW, elemH}, 0.2f, 8, 2.0f, borderColor);
        
        std::string valStr = std::to_string(data_[i]);
        DrawText(valStr.c_str(), static_cast<int>(startX + elemW/2 - MeasureText(valStr.c_str(), 20)/2), static_cast<int>(ey + elemH/2 - 10), 20, CYBER_TEXT);
    }

    if (popping_) {
        float ey = bottomY - (data_.size() + 1) * (elemH + spacing);
        ey -= animProgress_ * 50.0f;
        Color c = CYBER_PANEL; c.a = static_cast<unsigned char>(255 * (1.0f - animProgress_));
        Color tc = CYBER_TEXT; tc.a = c.a;
        Color bc = CYBER_CYAN; bc.a = c.a;
        
        DrawRectangleRounded({startX, ey, elemW, elemH}, 0.2f, 8, c);
        DrawRectangleRoundedLinesEx({startX, ey, elemW, elemH}, 0.2f, 8, 2.0f, bc);
        std::string valStr = std::to_string(animatingValue_);
        DrawText(valStr.c_str(), static_cast<int>(startX + elemW/2 - MeasureText(valStr.c_str(), 20)/2), static_cast<int>(ey + elemH/2 - 10), 20, tc);
    }
}
