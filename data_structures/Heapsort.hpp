#pragma once

#include <vector>

template <typename T>
class Heapsort {
    static void sink(std::vector<T>& arr, int k, const int n) {
        while (2 * k <= n) {                    // While left child exists
            int j = 2 * k;                      // Left child index (1-based)

            // Choose the larger child
            if (j < n && arr[j - 1] < arr[j]) { // If right child exists and is larger
                j++;                            // Select right child
            }

            // Check if heap property is satisfied
            if (!(arr[k - 1] < arr[j - 1])) {   // If parent >= larger child
                break;                          // Heap property satisfied
            }

            // Swap parent with larger child and move down
            std::swap(arr[k - 1], arr[j - 1]);
            k = j;                              // Move to child position
        }
    }

public:
    Heapsort() = delete;

    static void sort(std::vector<T>& arr) {
        const int n = arr.size();

        // Handle edge cases
        if (n <= 1) return;

        // Phase 1: Heapify - Build max-heap from bottom up
        // Start from last non-leaf node and work backwards to root
        for (int k = n / 2; k >= 1; --k) {
            sink(arr, k, n);
        }

        // Phase 2: Sort-down - Extract maximum elements
        // Repeatedly move the maximum element to its final position
        int k = n;
        while (k > 1) {
            // Move current maximum (root) to sorted position
            std::swap(arr[0], arr[--k]);      // arr[0] is root, arr[k-1] is last unsorted
            // Restore heap property for reduced heap
            sink(arr, 1, k);                    // Sink new root down
        }
    }
};