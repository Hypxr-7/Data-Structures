#pragma once

#include <vector>

template <typename T>
class InsertionSortOptimized {
public:
    InsertionSortOptimized() = delete;

    static void sort(std::vector<T>& arr) {
        const int n = static_cast<int>(arr.size());

        // Early termination optimization: single pass to detect sorted array
        int exchanges = 0;
        for (int i = n - 1; i > 0; --i) {
            if (arr[i] < arr[i - 1]) {
                std::swap(arr[i], arr[i - 1]);
                ++exchanges;
            }
        }

        // If no exchanges were made, array is already sorted
        if (exchanges == 0) return;

        // Perform optimized insertion sort with reduced write operations
        for (int i = 2; i < n; ++i) {
            T temp = arr[i];
            int j = i;

            // Shift elements instead of swapping to reduce write operations
            while (j > 0 && temp < arr[j - 1]) {
                arr[j] = arr[j - 1];
                --j;
            }

            // Place the element in its correct position
            arr[j] = temp;
        }
    }
};