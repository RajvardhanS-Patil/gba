#pragma once
// ═══════════════════════════════════════════════════════════════════════════
// RUNTIME: The Last Algorithm
// Random — Random number generation utilities
// ═══════════════════════════════════════════════════════════════════════════

#include <random>
#include <vector>
#include <algorithm>

class Random {
public:
    // Singleton — uses a thread-local generator
    static Random& instance();

    Random(const Random&) = delete;
    Random& operator=(const Random&) = delete;

    // Seed the generator
    void seed(unsigned int s);

    // Generate random integers in [min, max]
    int range(int min, int max);

    // Generate random float in [min, max]
    float rangeF(float min, float max);

    // Generate random float in [0, 1]
    float value();

    // Random boolean with given probability of true
    bool chance(float probability);

    // Pick random element from vector
    template<typename T>
    const T& pick(const std::vector<T>& vec) {
        return vec[range(0, static_cast<int>(vec.size()) - 1)];
    }

    // Shuffle a vector
    template<typename T>
    void shuffle(std::vector<T>& vec) {
        std::shuffle(vec.begin(), vec.end(), generator_);
    }

private:
    Random();

    std::mt19937 generator_;
};
