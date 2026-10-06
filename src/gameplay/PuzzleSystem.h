#pragma once
#include <string>
#include <vector>

enum class PuzzleState { INACTIVE, PRESENTING, SOLVING, COMPLETE, FAILED };

struct PuzzleResult {
    int correctOps;
    int totalOps;
    float timeTaken;
    float efficiency;
    std::string concept;
    std::string takeaway;
};

class PuzzleSystem {
public:
    PuzzleSystem();
    ~PuzzleSystem() = default;

    void startArraySearchPuzzle(std::vector<int> data, int target);
    void update(float dt);
    void draw();

    void checkAnswer(int index);
    PuzzleResult getResult() const;
    bool isActive() const;

private:
    PuzzleState state_;
    std::string puzzleType_;
    std::vector<int> puzzleData_;
    int targetValue_;
    float timer_;
    int operations_;
    int correctOps_;
};
