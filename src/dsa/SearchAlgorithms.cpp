#include "SearchAlgorithms.h"

int LinearSearch::search(const std::vector<int>& data, int target) {
    steps_.clear();
    currentStep_ = 0;
    int comparisons = 0;
    
    for (int i = 0; i < data.size(); i++) {
        comparisons++;
        bool matched = (data[i] == target);
        steps_.push_back({i, matched, comparisons});
        if (matched) return i;
    }
    return -1;
}

void LinearSearch::nextStep() {
    if (currentStep_ < steps_.size()) currentStep_++;
}

std::vector<SearchStep> LinearSearch::getSteps() const { return steps_; }


int BinarySearch::search(const std::vector<int>& data, int target) {
    steps_.clear();
    currentStep_ = 0;
    int comparisons = 0;
    
    int left = 0, right = data.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        comparisons++;
        
        if (data[mid] == target) {
            steps_.push_back({left, right, mid, true, comparisons});
            return mid;
        }
        
        steps_.push_back({left, right, mid, false, comparisons});
        
        if (data[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

std::vector<BinarySearch::BinStep> BinarySearch::getSteps() const { return steps_; }
