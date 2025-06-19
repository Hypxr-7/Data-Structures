#pragma once

#include <vector>

template <typename T>
class ShellSort {
public:
    ShellSort() = delete;

    static void sort(std::vector<T>& arr) {
        const int n = static_cast<int>(arr.size());

        // Handle edge cases
        if (n <= 1) return;  // Array with 0 or 1 element is already sorted

        // Generate initial gap using Knuth's sequence: h = 3*h + 1
        // This creates the sequence: 1, 4, 13, 40, 121, 364, 1093, 3280, ...
        // We want the largest gap that is less than n/3 for optimal performance
        int h = 1;
        while (h < n / 3) {
            h = 3 * h + 1;
        }

        // Perform insertion sort with decreasing gaps
        // Each iteration h-sorts the array, making it more ordered
        while (h >= 1) {
            // h-sort the array (insertion sort with gap h)
            // This creates h interleaved sorted subsequences
            for (int i = h; i < n; ++i) {
                // Insert arr[i] into the sorted subsequence:
                // arr[i-h], arr[i-2h], arr[i-3h], ..., arr[i-kh] where i-kh >= 0
                // This is standard insertion sort but with gap h instead of 1
                for (int j = i; j >= h && arr[j] < arr[j - h]; j -= h) {
                    std::swap(arr[j], arr[j - h]);
                }
            }

            // Reduce gap for next iteration following Knuth's sequence
            // Since we built the sequence with h = 3*h + 1, we reverse with h = h/3
            h /= 3;
        }

        // After the final pass with h=1, the array is completely sorted
        // The h=1 pass is essentially insertion sort on a nearly sorted array,
        // which runs in nearly linear time due to the pre-sorting from larger gaps
    }
};