#include "PuzzleSystem.h"
#include <raylib.h>

PuzzleSystem::PuzzleSystem() : state_(PuzzleState::INACTIVE), targetValue_(0), timer_(0.0f), operations_(0), correctOps_(0) {}

void PuzzleSystem::startArraySearchPuzzle(std::vector<int> data, int target) {
    puzzleType_ = "ARRAY_SEARCH";
    puzzleData_ = data;
    targetValue_ = target;
    state_ = PuzzleState::PRESENTING;
    timer_ = 0.0f;
    operations_ = 0;
    correctOps_ = 0;
}

void PuzzleSystem::update(float dt) {
    if (state_ == PuzzleState::INACTIVE) return;

    if (state_ == PuzzleState::PRESENTING) {
        timer_ += dt;
        if (timer_ > 2.0f) {
            state_ = PuzzleState::SOLVING;
            timer_ = 0.0f;
        }
    } else if (state_ == PuzzleState::SOLVING) {
        timer_ += dt;
        // In a real implementation, mouse click input would trigger checkAnswer
        // For simplicity, we just simulate handling here or delegate to a handleInput.
    } else if (state_ == PuzzleState::COMPLETE || state_ == PuzzleState::FAILED) {
        timer_ += dt;
        if (timer_ > 3.0f) {
            state_ = PuzzleState::INACTIVE;
        }
    }
}

void PuzzleSystem::draw() {
    if (state_ == PuzzleState::INACTIVE) return;

    DrawRectangle(100, 100, GetScreenWidth() - 200, GetScreenHeight() - 200, Fade(DARKGRAY, 0.9f));
    DrawText(("Puzzle: " + puzzleType_).c_str(), 120, 120, 20, WHITE);
    
    if (state_ == PuzzleState::PRESENTING) {
        DrawText("Analyze the data...", 120, 160, 20, YELLOW);
    } else if (state_ == PuzzleState::SOLVING) {
        DrawText(("Find: " + std::to_string(targetValue_)).c_str(), 120, 160, 20, GREEN);
        
        for (size_t i = 0; i < puzzleData_.size(); ++i) {
            Rectangle rect = {120.0f + i * 60.0f, 220.0f, 50.0f, 50.0f};
            DrawRectangleRec(rect, LIGHTGRAY);
            DrawText(std::to_string(puzzleData_[i]).c_str(), rect.x + 15, rect.y + 15, 20, BLACK);
        }
    } else if (state_ == PuzzleState::COMPLETE) {
        DrawText("SUCCESS!", GetScreenWidth()/2 - 50, GetScreenHeight()/2, 40, GREEN);
    } else if (state_ == PuzzleState::FAILED) {
        DrawText("FAILED!", GetScreenWidth()/2 - 50, GetScreenHeight()/2, 40, RED);
    }
}

void PuzzleSystem::checkAnswer(int index) {
    if (state_ != PuzzleState::SOLVING) return;
    operations_++;
    
    if (index >= 0 && index < puzzleData_.size() && puzzleData_[index] == targetValue_) {
        correctOps_ = 1;
        state_ = PuzzleState::COMPLETE;
        timer_ = 0.0f;
    } else {
        if (operations_ >= puzzleData_.size()) {
            state_ = PuzzleState::FAILED;
            timer_ = 0.0f;
        }
    }
}

PuzzleResult PuzzleSystem::getResult() const {
    PuzzleResult res;
    res.correctOps = correctOps_;
    res.totalOps = operations_;
    res.timeTaken = timer_;
    res.efficiency = (operations_ > 0) ? (1.0f / operations_) : 0.0f;
    res.concept = puzzleType_;
    res.takeaway = (state_ == PuzzleState::COMPLETE) ? "O(N) search completed successfully." : "Search failed. Next time, check every element.";
    return res;
}

bool PuzzleSystem::isActive() const { return state_ != PuzzleState::INACTIVE; }
