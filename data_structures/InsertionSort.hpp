#pragma once

#include <vector>

template <typename T>
class InsertionSort {
public:
    InsertionSort() = delete;

    static void sort(std::vector<T>& arr) {
        const int n = static_cast<int>(arr.size());

        // Start from the second element (index 1) since first is trivially sorted
        for (int i = 1; i < n; ++i) {
            // Insert arr[i] into the sorted portion arr[0...i-1]
            for (int j = i; j > 0 && arr[j] < arr[j-1]; --j) {
                std::swap(arr[j], arr[j-1]);
            }
        }
    }
};
