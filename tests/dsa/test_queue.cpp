#include "dsa/Queue.h"
#include <iostream>
#include <cassert>

void runQueueTests() {
    std::cout << "[TEST] Running Queue Tests..." << std::endl;

    Queue q;
    assert(q.empty());
    assert(q.size() == 0);
    assert(q.front() == -1);   // Safe fallback on empty queue
    assert(q.dequeue() == -1); // Safe fallback on empty queue

    // Enqueue elements (FIFO)
    q.enqueue(10);
    assert(!q.empty());
    assert(q.size() == 1);
    assert(q.front() == 10);

    q.enqueue(20);
    q.enqueue(30);
    assert(q.size() == 3);
    assert(q.front() == 10);

    // Dequeue elements in FIFO order
    assert(q.dequeue() == 10);
    assert(q.front() == 20);
    assert(q.size() == 2);

    assert(q.dequeue() == 20);
    assert(q.dequeue() == 30);
    assert(q.empty());
    assert(q.dequeue() == -1);

    // Search in queue
    q.enqueue(55);
    q.enqueue(77);
    assert(q.search(55) == true);
    assert(q.search(77) == true);
    assert(q.search(999) == false);

    // Reset
    q.reset();
    assert(q.empty());
    assert(q.size() == 0);

    std::cout << "  -> Queue Tests Passed!" << std::endl;
}
