#pragma once

#include <algorithm>
#include <random>
#include <vector>

template <typename T>
class QuicksortHoare {
    static void quicksort(std::vector<T>& arr, const int lo, const int hi) {
        if (hi <= lo) return;  // Base case: subarray has 0 or 1 element

        // Partition the array and get the partition index
        const int j = partition(arr, lo, hi);

        // Recursively sort both partitions
        // Important: Include j in left partition for Hoare scheme
        quicksort(arr, lo, j);        // Sort left partition (includes j)
        quicksort(arr, j + 1, hi);    // Sort right partition (excludes j)
    }

    static int partition(std::vector<T>& arr, const int lo, const int hi) {
        // Median-of-three pivot selection for better performance
        int mid = lo + (hi - lo) / 2;

        // Sort arr[lo], arr[mid], arr[hi] so that arr[mid] becomes the median
        // This three-element sort ensures median-of-three selection
        if (arr[hi] < arr[lo]) std::swap(arr[lo], arr[hi]);
        if (arr[mid] < arr[lo]) std::swap(arr[lo], arr[mid]);
        if (arr[hi] < arr[mid]) std::swap(arr[mid], arr[hi]);

        T pivot = arr[mid];  // Use the median as pivot value

        int i = lo - 1;      // Left pointer (will be incremented before first use)
        int j = hi + 1;      // Right pointer (will be decremented before first use)

        // Main partitioning loop using Hoare's two-pointer technique
        while (true) {
            // Move left pointer right until we find element ≥ pivot
            do {
                ++i;
            } while (arr[i] < pivot);

            // Move right pointer left until we find element ≤ pivot
            do {
                --j;
            } while (pivot < arr[j]);

            // If pointers have crossed, partitioning is complete
            if (i >= j) return j;

            // Swap the out-of-place elements
            std::swap(arr[i], arr[j]);
        }
    }

public:
    QuicksortHoare() = delete;

    static void sort(std::vector<T>& arr) {
        if (arr.empty()) return;  // Handle empty array edge case

        // Shuffle array randomly to avoid worst-case performance on sorted inputs
        // This preprocessing step is crucial for maintaining probabilistic guarantees
        std::shuffle(arr.begin(), arr.end(), std::mt19937(std::random_device{}()));

        // Perform Hoare quicksort on the entire array
        quicksort(arr, 0, static_cast<int>(arr.size()) - 1);
    }
};