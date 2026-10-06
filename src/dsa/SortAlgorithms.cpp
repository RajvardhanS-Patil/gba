#include "SortAlgorithms.h"
#include <utility>

std::vector<SortStep> BubbleSort::sort(std::vector<int> data) {
    std::vector<SortStep> steps;
    int n = data.size();
    bool swapped;
    for (int i = 0; i < n - 1; i++) {
        swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            bool s = false;
            if (data[j] > data[j + 1]) {
                std::swap(data[j], data[j + 1]);
                swapped = true;
                s = true;
            }
            steps.push_back({data, j, j+1, s, s ? "Swapped" : "Compared"});
        }
        if (!swapped) break;
    }
    steps.push_back({data, -1, -1, false, "Sorted"});
    return steps;
}

std::vector<SortStep> SelectionSort::sort(std::vector<int> data) {
    std::vector<SortStep> steps;
    int n = data.size();
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            steps.push_back({data, min_idx, j, false, "Comparing min with curr"});
            if (data[j] < data[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            std::swap(data[min_idx], data[i]);
            steps.push_back({data, i, min_idx, true, "Swapped to put min in place"});
        }
    }
    steps.push_back({data, -1, -1, false, "Sorted"});
    return steps;
}

std::vector<SortStep> InsertionSort::sort(std::vector<int> data) {
    std::vector<SortStep> steps;
    int n = data.size();
    for (int i = 1; i < n; i++) {
        int key = data[i];
        int j = i - 1;
        while (j >= 0 && data[j] > key) {
            steps.push_back({data, j, j+1, false, "Comparing"});
            data[j + 1] = data[j];
            j = j - 1;
            steps.push_back({data, j+1, j+2, true, "Shifted"});
        }
        data[j + 1] = key;
        steps.push_back({data, j+1, -1, false, "Inserted"});
    }
    steps.push_back({data, -1, -1, false, "Sorted"});
    return steps;
}

void MergeSort::merge(std::vector<int>& arr, int l, int m, int r, std::vector<SortStep>& steps) {
    int n1 = m - l + 1, n2 = r - m;
    std::vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
    
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) { arr[k] = L[i]; i++; }
        else { arr[k] = R[j]; j++; }
        steps.push_back({arr, k, -1, true, "Merged element"});
        k++;
    }
    while (i < n1) { arr[k] = L[i]; i++; k++; steps.push_back({arr, k-1, -1, true, "Merged left remaining"}); }
    while (j < n2) { arr[k] = R[j]; j++; k++; steps.push_back({arr, k-1, -1, true, "Merged right remaining"}); }
}
void MergeSort::mergeSort(std::vector<int>& arr, int l, int r, std::vector<SortStep>& steps) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m, steps);
    mergeSort(arr, m + 1, r, steps);
    merge(arr, l, m, r, steps);
}
std::vector<SortStep> MergeSort::sort(std::vector<int> data) {
    std::vector<SortStep> steps;
    mergeSort(data, 0, data.size() - 1, steps);
    steps.push_back({data, -1, -1, false, "Sorted"});
    return steps;
}

int QuickSort::partition(std::vector<int>& arr, int low, int high, std::vector<SortStep>& steps) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        steps.push_back({arr, j, high, false, "Comparing to pivot"});
        if (arr[j] < pivot) {
            i++;
            std::swap(arr[i], arr[j]);
            steps.push_back({arr, i, j, true, "Swapped"});
        }
    }
    std::swap(arr[i + 1], arr[high]);
    steps.push_back({arr, i+1, high, true, "Pivot placed"});
    return (i + 1);
}
void QuickSort::quickSort(std::vector<int>& arr, int low, int high, std::vector<SortStep>& steps) {
    if (low < high) {
        int pi = partition(arr, low, high, steps);
        quickSort(arr, low, pi - 1, steps);
        quickSort(arr, pi + 1, high, steps);
    }
}
std::vector<SortStep> QuickSort::sort(std::vector<int> data) {
    std::vector<SortStep> steps;
    quickSort(data, 0, data.size() - 1, steps);
    steps.push_back({data, -1, -1, false, "Sorted"});
    return steps;
}
