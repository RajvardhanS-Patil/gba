#pragma once
#include "IDataStructure.h"

class LinkedList : public IDataStructure {
private:
    struct Node {
        int data;
        Node* next;
        Node(int d) : data(d), next(nullptr) {}
    };
    Node* head_ = nullptr;
    int size_ = 0;
    int highlightIndex_ = -1;

public:
    LinkedList() = default;
    ~LinkedList() override;

    void insert(int value) override;
    bool remove(int value) override;
    bool search(int value) const override;
    void visualize(float x, float y, float w, float h) const override;
    std::string getName() const override;
    std::string getComplexityInfo() const override;
    size_t size() const override;
    bool empty() const override;
    void reset() override;

    void insertFront(int value);
    void insertAt(int index, int value);
    int get(int index) const;
    void setHighlight(int index);
};
