#include "dsa/SearchAlgorithms.h"
#include "dsa/SortAlgorithms.h"
#include <iostream>
#include <cassert>
#include <vector>

void runAlgorithmTests() {
    std::cout << "[RUNNING] Search & Sort Algorithm Tests..." << std::endl;

    // Linear Search
    LinearSearch linSearch;
    std::vector<int> data = {14, 28, 42, 56, 70};
    int foundIdx = linSearch.search(data, 42);
    assert(foundIdx == 2);
    auto linSteps = linSearch.getSteps();
    assert(!linSteps.empty());
    assert(linSteps.back().matched == true);
    assert(linSteps.back().index == 2);

    int notFoundIdx = linSearch.search(data, 99);
    assert(notFoundIdx == -1);

    // Binary Search
    BinarySearch binSearch;
    std::vector<int> sortedData = {10, 20, 30, 40, 50, 60, 70};
    int binIdx = binSearch.search(sortedData, 50);
    assert(binIdx == 4);
    auto binSteps = binSearch.getSteps();
    assert(!binSteps.empty());
    assert(binSteps.back().matched == true);
    assert(binSteps.back().mid == 4);

    // Bubble Sort
    std::vector<int> unsorted = {64, 34, 25, 12, 22, 11, 90};
    auto bubbleSteps = BubbleSort::sort(unsorted);
    assert(!bubbleSteps.empty());
    assert(bubbleSteps.back().state == (std::vector<int>{11, 12, 22, 25, 34, 64, 90}));

    // Quick Sort
    unsorted = {64, 34, 25, 12, 22, 11, 90};
    auto quickSteps = QuickSort::sort(unsorted);
    assert(!quickSteps.empty());
    assert(quickSteps.back().state == (std::vector<int>{11, 12, 22, 25, 34, 64, 90}));

    // Insertion Sort
    unsorted = {64, 34, 25, 12, 22, 11, 90};
    auto insertSteps = InsertionSort::sort(unsorted);
    assert(!insertSteps.empty());
    assert(insertSteps.back().state == (std::vector<int>{11, 12, 22, 25, 34, 64, 90}));

    // Selection Sort
    unsorted = {64, 34, 25, 12, 22, 11, 90};
    auto selectSteps = SelectionSort::sort(unsorted);
    assert(!selectSteps.empty());
    assert(selectSteps.back().state == (std::vector<int>{11, 12, 22, 25, 34, 64, 90}));

    // Merge Sort
    unsorted = {64, 34, 25, 12, 22, 11, 90};
    auto mergeSteps = MergeSort::sort(unsorted);
    assert(!mergeSteps.empty());
    assert(mergeSteps.back().state == (std::vector<int>{11, 12, 22, 25, 34, 64, 90}));

    std::cout << "  ✓ Search & Sort algorithm tests passed" << std::endl;
}
