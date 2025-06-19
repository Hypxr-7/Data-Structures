#pragma once

#include <vector>

template <typename T>
class InsertionSortBinary {
public:
    InsertionSortBinary() = delete;

    static void sort(std::vector<T>& arr) {
        const int n = static_cast<int>(arr.size());

        for (int i = 1; i < n; ++i) {
            T temp = arr[i];
            int lo = 0;
            int hi = i;

            // Binary search to find insertion position
            while (lo < hi) {
                int mid = lo + (hi - lo) / 2;
                if (temp < arr[mid]) {
                    hi = mid;
                } else {
                    lo = mid + 1;
                }
            }

            // Shift elements to make room for insertion
            for (int j = i; j > lo; --j) {
                arr[j] = arr[j - 1];
            }

            // Insert the element at the found position
            arr[lo] = temp;
        }
    }
};