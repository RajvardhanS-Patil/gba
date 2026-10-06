#include "dsa/Stack.h"
#include <iostream>
#include <cassert>

void runStackTests() {
    std::cout << "[TEST] Running Stack Tests..." << std::endl;

    Stack s;
    assert(s.empty());
    assert(s.size() == 0);
    assert(s.peek() == -1); // Safe fallback on empty stack
    assert(s.pop() == -1);  // Safe fallback on empty stack

    // Push elements (LIFO)
    s.push(100);
    assert(!s.empty());
    assert(s.size() == 1);
    assert(s.peek() == 100);

    s.push(200);
    s.push(300);
    assert(s.size() == 3);
    assert(s.peek() == 300);

    // Pop elements in reverse order
    assert(s.pop() == 300);
    assert(s.peek() == 200);
    assert(s.size() == 2);

    assert(s.pop() == 200);
    assert(s.pop() == 100);
    assert(s.empty());
    assert(s.pop() == -1);

    // Test search inside stack
    s.push(42);
    s.push(84);
    assert(s.search(42) == true);
    assert(s.search(999) == false);

    // Test remove(value)
    s.remove(84); // Top of stack
    assert(s.peek() == 42);

    std::cout << "  -> Stack Tests Passed!" << std::endl;
}
