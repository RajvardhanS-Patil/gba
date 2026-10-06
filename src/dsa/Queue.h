#pragma once
#include "IDataStructure.h"
#include <vector>

class Queue : public IDataStructure {
private:
    std::vector<int> data_;
    float animProgress_ = 1.0f;
    bool enqueueing_ = false;
    bool dequeueing_ = false;
    int animatingValue_ = 0;

public:
    Queue() = default;

    void insert(int value) override; // enqueue
    bool remove(int value) override; // dequeue if matches
    bool search(int value) const override;
    void visualize(float x, float y, float w, float h) const override;
    std::string getName() const override;
    std::string getComplexityInfo() const override;
    size_t size() const override;
    bool empty() const override;
    void reset() override;

    void enqueue(int value);
    int dequeue();
    int front() const;
    void updateAnim(float dt);
};
