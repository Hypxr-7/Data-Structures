#pragma once

#include <algorithm>
#include <random>
#include <vector>

template <typename T>
class QuicksortThreeWay {
    static void quicksort(std::vector<T>& arr, int lo, const int hi) {
        if (hi <= lo) return;  // Base case: subarray has 0 or 1 element

        // Initialize pointers for three-way partitioning
        int lt = lo;           // Boundary: arr[lo..lt-1] < pivot
        int gt = hi;           // Boundary: arr[gt+1..hi] > pivot
        T pivot = arr[lo];     // Use first element as pivot
        int i = lo + 1;        // Current position: arr[lt..i-1] == pivot

        // Three-way partitioning loop (Dutch National Flag algorithm)
        while (i <= gt) {
            if (arr[i] < pivot) {
                // Element belongs in < section
                // Swap with element at lt boundary and advance both pointers
                std::swap(arr[lt++], arr[i++]);
            }
            else if (arr[i] > pivot) {
                // Element belongs in > section
                // Swap with element at gt boundary, but don't advance i
                // (need to examine the swapped element)
                std::swap(arr[i], arr[gt--]);
            }
            else {
                // Element equals pivot, already in correct region
                // Just advance to next element
                ++i;
            }
        }

        // Recursively sort the sections that don't equal the pivot
        // The equal section (arr[lt..gt]) is already in its final position
        quicksort(arr, lo, lt - 1);    // Sort elements < pivot
        quicksort(arr, gt + 1, hi);    // Sort elements > pivot
        // Note: Elements from lt to gt are equal to pivot and don't need sorting
    }

public:
    QuicksortThreeWay() = delete;

    static void sort(std::vector<T>& arr) {
        if (arr.empty()) return;  // Handle empty array edge case

        // Shuffle array randomly to avoid worst-case performance on sorted inputs
        // This is especially important for three-way quicksort to ensure good pivot selection
        std::shuffle(arr.begin(), arr.end(), std::mt19937(std::random_device{}()));

        // Perform three-way quicksort on the entire array
        quicksort(arr, 0, static_cast<int>(arr.size()) - 1);
    }
};
