#pragma once
#include "IDataStructure.h"
#include <vector>

class ArrayDS : public IDataStructure {
private:
    std::vector<int> data_;
    int highlightIndex_ = -1;
    int searchIndex_ = -1;
    mutable int lastAccessCost_ = 0;

public:
    ArrayDS() = default;
    
    void insert(int value) override;
    bool remove(int value) override;
    bool search(int value) const override;
    void visualize(float x, float y, float w, float h) const override;
    std::string getName() const override;
    std::string getComplexityInfo() const override;
    size_t size() const override;
    bool empty() const override;
    void reset() override;

    void insertAt(int index, int value);
    void removeAt(int index);
    int get(int index) const;
    void setHighlight(int index);
    void setSearchProgress(int index);
    int getLastAccessCost() const;
};
