#pragma once

#include <algorithm>
#include <random>
#include <vector>

template <typename T>
class QuicksortDualPivot {
    static void quicksort(std::vector<T>& arr, int lo, int hi) {
        if (hi <= lo) return;  // Base case: subarray has 0 or 1 element

        // Ensure arr[lo] <= arr[hi] (left pivot <= right pivot)
        if (arr[hi] < arr[lo]) {
            std::swap(arr[hi], arr[lo]);
        }

        T pivot1 = arr[lo];    // Smaller pivot (left)
        T pivot2 = arr[hi];    // Larger pivot (right)

        // Initialize partitioning pointers
        int lt = lo + 1;       // Boundary for elements < pivot1
        int gt = hi - 1;       // Boundary for elements > pivot2
        int i = lo + 1;        // Current element being examined

        // Three-way partitioning with dual pivots
        // Invariant: arr[lo+1..lt-1] < pivot1 <= arr[lt..i-1] <= pivot2 < arr[gt+1..hi-1]
        while (i <= gt) {
            if (arr[i] < pivot1) {
                // Element belongs in < pivot1 region
                std::swap(arr[lt++], arr[i++]);
            }
            else if (arr[i] > pivot2) {
                // Element belongs in > pivot2 region
                // Note: Don't increment i here, need to examine the swapped element
                std::swap(arr[i], arr[gt--]);
            }
            else {
                // Element is between pivots (pivot1 <= arr[i] <= pivot2)
                ++i;
            }
        }

        // Place pivots in their correct final positions
        std::swap(arr[lo], arr[--lt]);    // Move pivot1 to its final position
        std::swap(arr[hi], arr[++gt]);    // Move pivot2 to its final position

        // Recursively sort the three regions
        quicksort(arr, lo, lt - 1);       // Sort elements < pivot1

        // Only sort middle region if pivots are different (avoid unnecessary work)
        if (pivot1 < pivot2) {
            quicksort(arr, lt + 1, gt - 1);  // Sort elements between pivots
        }

        quicksort(arr, gt + 1, hi);       // Sort elements > pivot2
    }

public:
    QuicksortDualPivot() = delete;

    static void sort(std::vector<T>& arr) {
        if (arr.empty()) return;  // Handle empty array edge case

        // Shuffle array randomly to avoid worst-case performance on sorted inputs
        // This preprocessing step is crucial for maintaining probabilistic guarantees
        std::shuffle(arr.begin(), arr.end(), std::mt19937(std::random_device{}()));

        // Perform dual-pivot quicksort on the entire array
        quicksort(arr, 0, static_cast<int>(arr.size()) - 1);
    }
};
