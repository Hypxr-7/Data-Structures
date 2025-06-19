#pragma once

#include <algorithm>
#include <vector>

template <typename T>
class MergesortBottomUp {
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
    MergesortBottomUp() = delete;

    static void sort(std::vector<T>& arr) {
        if (arr.empty()) return;  // Handle empty array edge case

        int n = static_cast<int>(arr.size());
        std::vector<T> aux(n);    // Auxiliary array for merging

        // Iterate through subarray lengths: 1, 2, 4, 8, 16, ...
        // Each pass doubles the size of sorted subarrays
        for (int len = 1; len < n; len *= 2) {
            // Merge adjacent subarrays of length 'len'
            // Process array in chunks of size 2*len
            for (int lo = 0; lo < n - len; lo += 2 * len) {
                int mid = lo + len - 1;                    // End of first subarray
                int hi = std::min(lo + len + len - 1, n - 1);  // End of second subarray
                merge(arr, aux, lo, mid, hi);
            }
        }
    }
};