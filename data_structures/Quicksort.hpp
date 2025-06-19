#pragma once

#include <algorithm>
#include <random>
#include <vector>

template <typename T>
class Quicksort {
    static void quicksort(std::vector<T>& arr, const int lo, const int hi) {
        if (hi <= lo) return;  // Base case: subarray has 0 or 1 element

        // Partition the array and get the pivot's final position
        const int j = partition(arr, lo, hi);

        // Recursively sort subarrays on both sides of the pivot
        quicksort(arr, lo, j - 1);     // Sort left subarray (< pivot)
        quicksort(arr, j + 1, hi);     // Sort right subarray (> pivot)
    }

    static int partition(std::vector<T>& arr, const int lo, const int hi) {
        int i = lo;           // Left pointer (starts at beginning)
        int j = hi + 1;       // Right pointer (starts beyond right end)
        T pivot = arr[lo];    // Choose first element as pivot

        while (true) {
            // Find element on left side that should be on right (≥ pivot)
            while (arr[++i] < pivot) {
                if (i == hi) break;  // Prevent going out of bounds
            }

            // Find element on right side that should be on left (≤ pivot)
            while (pivot < arr[--j]) {
                if (j == lo) break;  // Prevent going out of bounds
            }

            // Check if pointers have crossed - partitioning is complete
            if (i >= j) break;

            // Swap the out-of-place elements
            std::swap(arr[i], arr[j]);
        }

        // Place pivot in its correct final position
        std::swap(arr[lo], arr[j]);

        return j;  // Return the final position of the pivot
    }

public:
    Quicksort() = delete;

    static void sort(std::vector<T>& arr) {
        if (arr.empty()) return;  // Handle empty array edge case

        // Shuffle array randomly to avoid worst-case performance on sorted inputs
        // This ensures probabilistic O(n log n) performance regardless of input order
        std::shuffle(arr.begin(), arr.end(), std::mt19937(std::random_device{}()));

        // Perform quicksort on the entire array
        quicksort(arr, 0, static_cast<int>(arr.size()) - 1);
    }
};
