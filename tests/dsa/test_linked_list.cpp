#include "dsa/LinkedList.h"
#include <iostream>
#include <cassert>

void runLinkedListTests() {
    std::cout << "[RUNNING] LinkedList Tests..." << std::endl;

    LinkedList list;
    assert(list.size() == 0);
    assert(list.empty());

    // Insert front and back (insert adds to back)
    list.insert(10);
    assert(list.size() == 1);
    assert(!list.empty());
    assert(list.get(0) == 10);

    list.insert(20);
    assert(list.size() == 2);
    assert(list.get(1) == 20);

    list.insertFront(5);
    assert(list.size() == 3);
    assert(list.get(0) == 5);
    assert(list.get(1) == 10);
    assert(list.get(2) == 20);

    // Insert at index
    list.insertAt(1, 7);
    assert(list.size() == 4);
    assert(list.get(0) == 5);
    assert(list.get(1) == 7);
    assert(list.get(2) == 10);
    assert(list.get(3) == 20);

    // Search
    assert(list.search(7) == true);
    assert(list.search(999) == false);

    // Remove by value
    bool removed = list.remove(7);
    assert(removed == true);
    assert(list.size() == 3);
    assert(list.get(1) == 10);

    // Reset / clear
    list.reset();
    assert(list.size() == 0);
    assert(list.empty());

    std::cout << "  ✓ LinkedList operations passed" << std::endl;
}
