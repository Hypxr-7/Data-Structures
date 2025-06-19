#pragma once

#include <algorithm>
#include <random>
#include <vector>

template <typename T>
class QuicksortBentleyMcIlroy {
    static constexpr int INSERTION_SORT_CUTOFF = 8;   // Switch to insertion sort below this size
    static constexpr int MEDIAN_OF_3_CUTOFF = 40;     // Switch to ninther above this size

    static void quicksort(std::vector<T>& arr, int lo, const int hi) {
        const int n = hi - lo + 1;

        // Optimization 1: Use insertion sort for small subarrays
        if (n <= INSERTION_SORT_CUTOFF) {
            insertionSort(arr, lo, hi);
            return;
        }

        // Optimization 2: Median-of-3 pivot selection for medium arrays
        if (n <= MEDIAN_OF_3_CUTOFF) {
            int m = median3(arr, lo, lo + n / 2, hi);
            std::swap(arr[m], arr[lo]);
        }

        // Optimization 3: Tukey ninther for large arrays
        else {
            const int eps = n / 8;
            const int mid = lo + n / 2;

            // Find medians of three triplets distributed across the array
            const int m1 = median3(arr, lo, lo + eps, lo + eps + eps);
            const int m2 = median3(arr, mid - eps, mid, mid + eps);
            const int m3 = median3(arr, hi - eps - eps, hi - eps, hi);

            // Find median of the three medians (ninther)
            int ninther = median3(arr, m1, m2, m3);
            std::swap(arr[ninther], arr[lo]);
        }

        // Bentley-McIlroy 3-way partitioning
        int i = lo;        // Left scanning pointer
        int j = hi + 1;    // Right scanning pointer
        int p = lo;        // Left boundary of equal elements
        int q = hi + 1;    // Right boundary of equal elements
        T v = arr[lo];     // Pivot value

        while (true) {
            // Scan from left for element >= pivot
            while (arr[++i] < v) {
                if (i == hi) break;
            }

            // Scan from right for element <= pivot
            while (v < arr[--j]) {
                if (j == lo) break;
            }

            // Handle the case where pointers meet on an element equal to pivot
            if (i == j && arr[i] == v) {
                std::swap(arr[++p], arr[i]);
            }

            // If pointers have crossed, partitioning is complete
            if (i >= j) break;

            // Swap elements that are on wrong sides
            std::swap(arr[i], arr[j]);

            // Move equal elements to the edges for later consolidation
            if (arr[i] == v) std::swap(arr[++p], arr[i]);
            if (arr[j] == v) std::swap(arr[--q], arr[j]);
        }

        // Move all equal elements to the center
        i = j + 1;

        // Move equal elements from left edge to center-left
        for (int k = lo; k <= p; ++k) {
            std::swap(arr[k], arr[j--]);
        }

        // Move equal elements from right edge to center-right
        for (int k = hi; k >= q; --k) {
            std::swap(arr[k], arr[i++]);
        }

        // Recursively sort the parts that don't equal the pivot
        quicksort(arr, lo, j);    // Sort elements < pivot
        quicksort(arr, i, hi);    // Sort elements > pivot
        // Elements from j+1 to i-1 equal pivot and are already in correct position
    }

    static void insertionSort(std::vector<T>& arr, const int lo, const int hi) {
        for (int i = lo + 1; i <= hi; ++i) {
            for (int j = i; j > lo && arr[j] < arr[j - 1]; --j) {
                std::swap(arr[j], arr[j - 1]);
            }
        }
    }

    static int median3(std::vector<T>& arr, int i, int j, int k) {
        return ((arr[i] < arr[j]) ? ((arr[j] < arr[k])   ? j      // i < j < k
                                     : (arr[i] < arr[k]) ? k      // i < k < j
                                                         : i)     // k < i < j
                                  : ((arr[k] < arr[j])   ? j      // k < j < i
                                     : (arr[k] < arr[i]) ? k      // j < k < i
                                                         : i));   // j < i < k
    }

public:
    QuicksortBentleyMcIlroy() = delete;

    static void sort(std::vector<T>& arr) {
        if (arr.empty()) return;  // Handle empty array edge case

        // Shuffle array randomly to avoid worst-case performance on sorted inputs
        // This is crucial for maintaining probabilistic O(n log n) performance
        std::shuffle(arr.begin(), arr.end(), std::mt19937(std::random_device{}()));

        // Perform Bentley-McIlroy quicksort on the entire array
        quicksort(arr, 0, static_cast<int>(arr.size()) - 1);
    }
};