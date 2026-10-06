#pragma once
#include <vector>
#include <string>

struct SortStep {
    std::vector<int> state;
    int compareIdx1;
    int compareIdx2;
    bool swapped;
    std::string description;
};

class BubbleSort {
public:
    static std::vector<SortStep> sort(std::vector<int> data);
};

class SelectionSort {
public:
    static std::vector<SortStep> sort(std::vector<int> data);
};

class InsertionSort {
public:
    static std::vector<SortStep> sort(std::vector<int> data);
};

class MergeSort {
private:
    static void merge(std::vector<int>& arr, int l, int m, int r, std::vector<SortStep>& steps);
    static void mergeSort(std::vector<int>& arr, int l, int r, std::vector<SortStep>& steps);
public:
    static std::vector<SortStep> sort(std::vector<int> data);
};

class QuickSort {
private:
    static int partition(std::vector<int>& arr, int low, int high, std::vector<SortStep>& steps);
    static void quickSort(std::vector<int>& arr, int low, int high, std::vector<SortStep>& steps);
public:
    static std::vector<SortStep> sort(std::vector<int> data);
};
