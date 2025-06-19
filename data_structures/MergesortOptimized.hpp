#pragma once

#include <vector>

template <typename T>
class MergesortOptimized {
    static constexpr int CUTOFF = 7;

    static void mergesort(std::vector<T>& src, std::vector<T>& dst, const int lo, const int hi) {
        // Optimization 1: Use insertion sort for small subarrays
        if (hi <= lo + CUTOFF) {
            insertionSort(dst, lo, hi);
            return;
        }

        int mid = lo + (hi - lo) / 2;

        // Recursively sort both halves, alternating src and dst
        // This eliminates the need for copying between arrays
        mergesort(dst, src, lo, mid);        // Sort left half: dst→src
        mergesort(dst, src, mid + 1, hi);    // Sort right half: dst→src

        // Optimization 2: Skip merge if already sorted
        // If the maximum of left subarray <= minimum of right subarray,
        // the entire range is already sorted
        if (src[mid] <= src[mid + 1]) {
            // Already sorted, just copy from src to dst
            for (int i = lo; i <= hi; ++i) {
                dst[i] = src[i];
            }
            return;
        }

        // Perform optimized merge operation
        merge(src, dst, lo, mid, hi);
    }

    static void merge(std::vector<T>& src, std::vector<T>& dst, const int lo, const int mid, const int hi) {
        int i = lo;        // Index for left subarray (lo..mid)
        int j = mid + 1;   // Index for right subarray (mid+1..hi)

        // Merge the two sorted subarrays directly from src to dst
        for (int k = lo; k <= hi; ++k) {
            if (i > mid) {
                // Left subarray exhausted, take remaining from right
                dst[k] = src[j++];
            }
            else if (j > hi) {
                // Right subarray exhausted, take remaining from left
                dst[k] = src[i++];
            }
            else if (src[j] < src[i]) {
                // Right element is smaller, take from right
                dst[k] = src[j++];
            }
            else {
                // Left element is smaller or equal, take from left
                // This maintains stability by preferring left element for ties
                dst[k] = src[i++];
            }
        }
    }

    static void insertionSort(std::vector<T>& arr, const int lo, const int hi) {
        // Insert each element into its correct position in the sorted portion
        for (int i = lo + 1; i <= hi; ++i) {
            // Move arr[i] to its correct position by swapping with smaller neighbors
            for (int j = i; j > lo && arr[j] < arr[j - 1]; --j) {
                std::swap(arr[j], arr[j - 1]);
            }
        }
    }

public:
    MergesortOptimized() = delete;

    static void sort(std::vector<T>& arr) {
        if (arr.empty()) return;  // Handle empty array edge case

        // Create auxiliary array as a copy of original
        // This serves as the alternating array for optimization
        const int n = static_cast<int>(arr.size());
        std::vector<T> aux = arr;

        // Start recursive sort with aux as source, arr as destination
        // This ensures the final result ends up in the original array
        mergesort(aux, arr, 0, n - 1);
    }
};
