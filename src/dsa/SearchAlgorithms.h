#pragma once
#include <vector>
#include <string>

struct SearchStep {
    int index;
    bool matched;
    int comparisonCount;
};

struct SearchVisualization {
    std::vector<SearchStep> steps;
    int target;
};

class LinearSearch {
private:
    std::vector<SearchStep> steps_;
    int currentStep_ = 0;
    
public:
    int search(const std::vector<int>& data, int target);
    void nextStep();
    std::vector<SearchStep> getSteps() const;
};

class BinarySearch {
private:
    struct BinStep {
        int left, right, mid;
        bool matched;
        int comparisonCount;
    };
    std::vector<BinStep> steps_;
    int currentStep_ = 0;
    
public:
    int search(const std::vector<int>& data, int target);
    std::vector<BinStep> getSteps() const;
};
