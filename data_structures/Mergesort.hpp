#pragma once

#include <vector>

template <typename T>
class Mergesort {
    static void mergesort(std::vector<T>& arr, std::vector<T>& aux, const int lo, const int hi) {
        // Base case: if subarray has 0 or 1 element, it's already sorted
        if (hi <= lo) return;

        // Divide: find the midpoint
        // Using lo + (hi - lo) / 2 to avoid integer overflow
        const int mid = lo + (hi - lo) / 2;

        // Conquer: recursively sort both halves
        mergesort(arr, aux, lo, mid);        // Sort left half: arr[lo..mid]
        mergesort(arr, aux, mid + 1, hi);    // Sort right half: arr[mid+1..hi]

        // Combine: merge the sorted halves
        merge(arr, aux, lo, mid, hi);
    }

    static void merge(std::vector<T>& arr, std::vector<T>& aux, const int lo, const int mid, const
     int hi) {
        // Copy subarray to auxiliary array for safe merging
        for (int k = lo; k <= hi; ++k) {
            aux[k] = arr[k];
        }

        int i = lo;        // Index for left subarray (lo..mid)
        int j = mid + 1;   // Index for right subarray (mid+1..hi)

        // Merge back to original array by comparing elements
        for (int k = lo; k <= hi; ++k) {
            if (i > mid) {
                // Left subarray exhausted, take remaining from right
                arr[k] = aux[j++];
            }
            else if (j > hi) {
                // Right subarray exhausted, take remaining from left
                arr[k] = aux[i++];
            }
            else if (aux[j] < aux[i]) {
                // Right element is smaller, take from right
                arr[k] = aux[j++];
            }
            else {
                // Left element is smaller or equal, take from left
                // This maintains stability by preferring left element for ties
                arr[k] = aux[i++];
            }
        }
    }

public:
    Mergesort() = delete;
    static void sort(std::vector<T>& arr) {
        if (arr.empty()) return;  // Handle empty array edge case

        int n = arr.size();
        std::vector<T> aux(n);    // Auxiliary array for merging
        mergesort(arr, aux, 0, n-1);  // Sort entire array
    }
};
