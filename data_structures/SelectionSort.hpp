#pragma once

#include <vector>

template <typename T>
class SelectionSort {
public:
    SelectionSort() = delete;

    static void sort(std::vector<T>& arr) {
        const int n = static_cast<int>(arr.size());

        // Handle edge cases
        if (n <= 1) return;  // Array with 0 or 1 element is already sorted

        // Main selection sort algorithm
        for (int i = 0; i < n - 1; ++i) {
            int minIndex = i;  // Assume first element of unsorted portion is minimum

            // Find the minimum element in the remaining unsorted portion
            // Search from i+1 to n-1 for element smaller than arr[minIndex]
            for (int j = i + 1; j < n; ++j) {
                if (arr[j] < arr[minIndex]) {
                    minIndex = j;  // Update index of minimum element
                }
            }

            // Swap the minimum element with the first element of unsorted portion
            // Optimization: Only perform swap if minimum is not already in position
            if (minIndex != i) {
                std::swap(arr[i], arr[minIndex]);
            }

            // At this point, arr[0..i] contains the i+1 smallest elements in sorted order
            // and arr[i+1..n-1] contains the remaining elements in arbitrary order
        }

        // After the loop, arr[0..n-1] is completely sorted in ascending order
    }
};