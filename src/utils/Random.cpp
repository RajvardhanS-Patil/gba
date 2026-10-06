// ═══════════════════════════════════════════════════════════════════════════
// RUNTIME: The Last Algorithm
// Random — Implementation
// ═══════════════════════════════════════════════════════════════════════════

#include "utils/Random.h"

Random& Random::instance() {
    static Random inst;
    return inst;
}

Random::Random()
    : generator_(std::random_device{}())
{
}

void Random::seed(unsigned int s) {
    generator_.seed(s);
}

int Random::range(int min, int max) {
    if (min > max) std::swap(min, max);
    std::uniform_int_distribution<int> dist(min, max);
    return dist(generator_);
}

float Random::rangeF(float min, float max) {
    if (min > max) std::swap(min, max);
    std::uniform_real_distribution<float> dist(min, max);
    return dist(generator_);
}

float Random::value() {
    return rangeF(0.0f, 1.0f);
}

bool Random::chance(float probability) {
    return value() < probability;
}
