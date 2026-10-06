#include "dsa/ArrayDS.h"
#include <iostream>
#include <cassert>

void runArrayTests() {
    std::cout << "[TEST] Running ArrayDS Tests..." << std::endl;

    ArrayDS arr;
    assert(arr.empty());
    assert(arr.size() == 0);

    // Test insert
    arr.insert(10);
    arr.insert(20);
    arr.insert(30);
    assert(arr.size() == 3);
    assert(!arr.empty());
    assert(arr.get(0) == 10);
    assert(arr.get(1) == 20);
    assert(arr.get(2) == 30);
    assert(arr.getLastAccessCost() == 1);

    // Out of bounds access
    assert(arr.get(-1) == -1);
    assert(arr.get(99) == -1);

    // Test insertAt
    arr.insertAt(1, 15); // [10, 15, 20, 30]
    assert(arr.size() == 4);
    assert(arr.get(1) == 15);
    assert(arr.get(2) == 20);

    // Test search (Linear Search)
    assert(arr.search(15) == true);
    assert(arr.search(999) == false);

    // Test remove by value
    bool removed = arr.remove(15);
    assert(removed == true);
    assert(arr.size() == 3);
    assert(arr.search(15) == false);

    // Remove non-existent
    assert(arr.remove(999) == false);

    // Test removeAt
    arr.removeAt(0); // removes 10 -> [20, 30]
    assert(arr.size() == 2);
    assert(arr.get(0) == 20);
    assert(arr.get(1) == 30);

    // Edge case: reset
    arr.reset();
    assert(arr.empty());
    assert(arr.size() == 0);
    assert(arr.search(20) == false);

    std::cout << "  -> ArrayDS Tests Passed!" << std::endl;
}
