#include "ArrayDS.h"
#include "raylib.h"
#include "ui/UISystem.h"
#include <string>
#include <cmath>

extern const Color CYBER_CYAN;
extern const Color CYBER_DARK;
extern const Color CYBER_PANEL;
extern const Color CYBER_TEXT;
extern const Color CYBER_DIM;
extern const Color CYBER_WARNING;
extern const Color CYBER_SUCCESS;
extern const Color CYBER_DANGER;

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
    float time = (float)GetTime();
    float cellW = 72;
    float cellH = 68;
    float padding = 12;
    float totalW = data_.size() * (cellW + padding) - padding;
    float startX = x + (w - totalW) / 2.0f;
    float startY = y + (h - cellH) / 2.0f;

    // Background container panel
    DrawRectangleRounded({startX - 18, startY - 26, totalW + 36, cellH + 52}, 0.12f, 6, Color{12, 16, 26, 220});
    DrawRectangleRoundedLinesEx({startX - 18, startY - 26, totalW + 36, cellH + 52}, 0.12f, 6, 1.5f, Color{35, 48, 72, 220});

    for (size_t i = 0; i < data_.size(); i++) {
        float cx = startX + i * (cellW + padding);
        Color cellColor = CYBER_PANEL;
        Color borderColor = Color{45, 60, 85, 255};
        Color textColor = CYBER_TEXT;

        bool isHighlighted = (static_cast<int>(i) == highlightIndex_);
        bool isSearching = (static_cast<int>(i) == searchIndex_);

        if (isHighlighted) {
            borderColor = CYBER_CYAN;
            cellColor = Color{20, 45, 65, 255};
            float pulse = (sinf(time * 8.0f) + 1.0f) * 0.5f;
            DrawRectangleRounded({cx - 3, startY - 3, cellW + 6, cellH + 6}, 0.2f, 8, Color{0, 229, 255, (unsigned char)(70 + pulse * 60)});
        } else if (isSearching) {
            borderColor = CYBER_WARNING;
            cellColor = Color{50, 40, 20, 255};
        }

        // Draw Cell
        DrawRectangleRounded({cx, startY, cellW, cellH}, 0.18f, 6, cellColor);
        DrawRectangleRoundedLinesEx({cx, startY, cellW, cellH}, 0.18f, 6, isHighlighted ? 2.5f : 1.5f, borderColor);

        // Memory byte offset simulation: e.g. 0x00, 0x04, 0x08...
        std::string memAddr = TextFormat("+0x%02X", (unsigned int)(i * 4));
        DrawText(memAddr.c_str(), (int)(cx + cellW/2 - MeasureText(memAddr.c_str(), 10)/2), (int)(startY + 6), 10, Color{90, 110, 140, 200});

        // Value inside cell
        std::string valStr = std::to_string(data_[i]);
        if (data_[i] == 999) { // Corrupted address in story
            valStr = "ERR!";
            textColor = CYBER_DANGER;
        }
        int tw = MeasureText(valStr.c_str(), 18);
        DrawText(valStr.c_str(), static_cast<int>(cx + cellW/2 - tw/2), static_cast<int>(startY + 26), 18, textColor);

        // Subtext: contiguous index badge
        std::string idxStr = "[" + std::to_string(i) + "]";
        Color idxCol = isHighlighted ? CYBER_CYAN : CYBER_DIM;
        DrawText(idxStr.c_str(), static_cast<int>(cx + cellW/2 - MeasureText(idxStr.c_str(), 12)/2), static_cast<int>(startY + cellH + 6), 12, idxCol);
    }
}
