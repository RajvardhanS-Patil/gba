#pragma once
#include "IDataStructure.h"
#include <vector>

class Stack : public IDataStructure {
private:
    std::vector<int> data_;
    int maxSize_ = 10;
    float animProgress_ = 1.0f;
    int animatingValue_ = 0;
    bool pushing_ = false;
    bool popping_ = false;

public:
    Stack() = default;

    void insert(int value) override; // Alias for push
    bool remove(int value) override;
    bool search(int value) const override;
    void visualize(float x, float y, float w, float h) const override;
    std::string getName() const override;
    std::string getComplexityInfo() const override;
    size_t size() const override;
    bool empty() const override;
    void reset() override;

    void push(int value);
    int pop();
    int peek() const;
    bool isFull() const;
    
    void updateAnim(float dt);
};
