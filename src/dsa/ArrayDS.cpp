#include "ArrayDS.h"
#include "raylib.h"
#include <string>

extern const Color CYBER_CYAN;
extern const Color CYBER_DARK;
extern const Color CYBER_PANEL;
extern const Color CYBER_TEXT;
extern const Color CYBER_DIM;
extern const Color CYBER_WARNING;

void ArrayDS::insert(int value) {
    data_.push_back(value);
    lastAccessCost_ = 1;
}

void ArrayDS::insertAt(int index, int value) {
    if (index >= 0 && static_cast<size_t>(index) <= data_.size()) {
        data_.insert(data_.begin() + index, value);
        lastAccessCost_ = static_cast<int>(data_.size() - index);
    }
}

bool ArrayDS::remove(int value) {
    lastAccessCost_ = 0;
    for (auto it = data_.begin(); it != data_.end(); ++it) {
        lastAccessCost_++;
        if (*it == value) {
            data_.erase(it);
            return true;
        }
    }
    return false;
}

void ArrayDS::removeAt(int index) {
    if (index >= 0 && static_cast<size_t>(index) < data_.size()) {
        data_.erase(data_.begin() + index);
        lastAccessCost_ = static_cast<int>(data_.size() - index);
    }
}

bool ArrayDS::search(int value) const {
    lastAccessCost_ = 0;
    for (size_t i = 0; i < data_.size(); i++) {
        lastAccessCost_++;
        if (data_[i] == value) {
            return true;
        }
    }
    return false;
}

int ArrayDS::get(int index) const {
    lastAccessCost_ = 1;
    return (index >= 0 && static_cast<size_t>(index) < data_.size()) ? data_[index] : -1;
}

void ArrayDS::setHighlight(int index) { highlightIndex_ = index; }
void ArrayDS::setSearchProgress(int index) { searchIndex_ = index; }
int ArrayDS::getLastAccessCost() const { return lastAccessCost_; }
std::string ArrayDS::getName() const { return "Array"; }
std::string ArrayDS::getComplexityInfo() const { return "Access: O(1) | Search: O(n) | Insert: O(n) | Delete: O(n)"; }
size_t ArrayDS::size() const { return data_.size(); }
bool ArrayDS::empty() const { return data_.empty(); }
void ArrayDS::reset() { data_.clear(); highlightIndex_ = -1; searchIndex_ = -1; lastAccessCost_ = 0; }

void ArrayDS::visualize(float x, float y, float w, float h) const {
    float cellW = 60;
    float cellH = 60;
    float padding = 10;
    float startX = x + (w - (data_.size() * (cellW + padding))) / 2.0f;
    float startY = y + (h - cellH) / 2.0f;

    for (size_t i = 0; i < data_.size(); i++) {
        float cx = startX + i * (cellW + padding);
        Color cellColor = CYBER_PANEL;
        Color borderColor = CYBER_DIM;

        if (static_cast<int>(i) == highlightIndex_) {
            borderColor = CYBER_CYAN;
            DrawRectangleRounded({cx-2, startY-2, cellW+4, cellH+4}, 0.2f, 8, {0, 229, 255, 100}); // Glow
        } else if (static_cast<int>(i) == searchIndex_) {
            borderColor = CYBER_WARNING;
        }

        DrawRectangleRounded({cx, startY, cellW, cellH}, 0.2f, 8, cellColor);
        DrawRectangleRoundedLinesEx({cx, startY, cellW, cellH}, 0.2f, 8, 2.0f, borderColor);

        // Value
        std::string valStr = std::to_string(data_[i]);
        int tw = MeasureText(valStr.c_str(), 20);
        DrawText(valStr.c_str(), static_cast<int>(cx + cellW/2 - tw/2), static_cast<int>(startY + cellH/2 - 10), 20, CYBER_TEXT);

        // Index
        std::string idxStr = std::to_string(i);
        DrawText(idxStr.c_str(), static_cast<int>(cx + cellW/2 - MeasureText(idxStr.c_str(), 10)/2), static_cast<int>(startY - 20), 10, CYBER_DIM);
    }
}
