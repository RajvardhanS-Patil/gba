#pragma once
#include <string>
#include <vector>

enum class ComplexityClass { O_1, O_LOG_N, O_N, O_N_LOG_N, O_N_SQUARED };

struct Record {
    std::string algorithmName;
    ComplexityClass complexity;
    int inputSize;
    int actualOps;
    float time;
};

class ComplexityAnalyzer {
private:
    std::vector<Record> records_;

public:
    static std::string toString(ComplexityClass c);
    static float evaluate(ComplexityClass c, int n);

    void addRecord(const Record& r);
    void visualize(float x, float y, float w, float h) const;
    const std::vector<Record>& getRecords() const;
};
