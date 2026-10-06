#include "ComplexityAnalyzer.h"
#include "raylib.h"
#include <cmath>

extern const Color CYBER_CYAN;
extern const Color CYBER_VIOLET;
extern const Color CYBER_PANEL;
extern const Color CYBER_TEXT;
extern const Color CYBER_DIM;
extern const Color CYBER_MAGENTA;

std::string ComplexityAnalyzer::toString(ComplexityClass c) {
    switch (c) {
        case ComplexityClass::O_1: return "O(1)";
        case ComplexityClass::O_LOG_N: return "O(log N)";
        case ComplexityClass::O_N: return "O(N)";
        case ComplexityClass::O_N_LOG_N: return "O(N log N)";
        case ComplexityClass::O_N_SQUARED: return "O(N^2)";
        default: return "Unknown";
    }
}

float ComplexityAnalyzer::evaluate(ComplexityClass c, int n) {
    if (n <= 0) return 0;
    switch (c) {
        case ComplexityClass::O_1: return 1.0f;
        case ComplexityClass::O_LOG_N: return std::log2(n);
        case ComplexityClass::O_N: return (float)n;
        case ComplexityClass::O_N_LOG_N: return n * std::log2(n);
        case ComplexityClass::O_N_SQUARED: return (float)(n * n);
        default: return 0.0f;
    }
}

void ComplexityAnalyzer::addRecord(const Record& r) { records_.push_back(r); }
const std::vector<Record>& ComplexityAnalyzer::getRecords() const { return records_; }

void ComplexityAnalyzer::visualize(float x, float y, float w, float h) const {
    DrawRectangleRounded({x, y, w, h}, 0.1f, 8, CYBER_PANEL);
    DrawRectangleRoundedLinesEx({x, y, w, h}, 0.1f, 8, 2.0f, CYBER_DIM);
    
    if (records_.empty()) {
        DrawText("No data to visualize.", x + w/2 - 100, y + h/2, 20, CYBER_DIM);
        return;
    }

    float padding = 40.0f;
    float graphW = w - padding * 2;
    float graphH = h - padding * 2;
    float originX = x + padding;
    float originY = y + h - padding;

    // Draw Axes
    DrawLineEx({originX, originY}, {originX + graphW, originY}, 2.0f, CYBER_TEXT);
    DrawLineEx({originX, originY}, {originX, originY - graphH}, 2.0f, CYBER_TEXT);

    int maxN = 10;
    int maxOps = 10;
    for (const auto& r : records_) {
        if (r.inputSize > maxN) maxN = r.inputSize;
        if (r.actualOps > maxOps) maxOps = r.actualOps;
    }

    float barW = (graphW / records_.size()) * 0.8f;
    float spacing = (graphW / records_.size()) * 0.2f;

    for (size_t i = 0; i < records_.size(); i++) {
        const auto& r = records_[i];
        float px = originX + spacing + i * (barW + spacing);
        float py = originY - ((float)r.actualOps / maxOps) * graphH;
        float pHeight = originY - py;
        
        Color barColor = (i % 2 == 0) ? CYBER_CYAN : CYBER_VIOLET;
        
        DrawRectangle(px, py, barW, pHeight, barColor);
        DrawText(r.algorithmName.c_str(), px, originY + 5, 10, CYBER_TEXT);
        std::string opStr = std::to_string(r.actualOps);
        DrawText(opStr.c_str(), px, py - 15, 10, CYBER_TEXT);
    }
}
