#pragma once
#include <string>

class IDataStructure {
public:
    virtual void insert(int value) = 0;
    virtual bool remove(int value) = 0;
    virtual bool search(int value) const = 0;
    virtual void visualize(float x, float y, float w, float h) const = 0;
    virtual std::string getName() const = 0;
    virtual std::string getComplexityInfo() const = 0;
    virtual size_t size() const = 0;
    virtual bool empty() const = 0;
    virtual void reset() = 0;
    virtual ~IDataStructure() = default;
};
