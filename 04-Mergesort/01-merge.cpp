#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

template<typename T>
void merge(std::vector<T>& arr, std::vector<T>& aux, int low, int mid, int high);
template<typename T>
void mergesort(std::vector<T>& arr, std::vector<T>& aux, int low, int high);
template<typename T>
void mergesortWrapper(std::vector<T>& arr);

void testMergesortWrapper();

int main() {
    testMergesortWrapper();

    return 0;
}

template<typename T>
void merge(std::vector<T>& arr, std::vector<T>& aux, int low, int mid, int high) {
    // copy to aux
    for (int k = low; k <= high; ++k)
        aux[k] = arr[k];

    // merge int arr
    int i = low, j = mid + 1;
    for (int k = low; k <= high; ++k) {
        if      (i > mid)         arr[k] = aux[j++];
        else if (j > high)        arr[k] = aux[i++];
        else if (aux[j] < aux[i]) arr[k] = aux[j++];
        else                      arr[k] = aux[i++];
    }
}

template<typename T>
void mergesort(std::vector<T>& arr, std::vector<T>& aux, int low, int high) {
    if (high <= low) return;

    int mid = low + (high - low) / 2;
    mergesort(arr, aux, low, mid);
    mergesort(arr, aux, mid + 1, high);
    merge(arr, aux, low, mid, high);
}

template<typename T>
void mergesortWrapper(std::vector<T>& arr) {
    std::vector<T> aux(arr.size());
    mergesort(arr, aux, 0, arr.size() - 1);
}

void testMergesortWrapper() {
    // Test case 1: Empty array
    std::vector<int> arr1 = {};
    mergesortWrapper(arr1);
    assert((arr1 == std::vector<int>{}));

    // Test case 2: Single element array
    std::vector<int> arr2 = {1};
    mergesortWrapper(arr2);
    assert((arr2 == std::vector<int>{1}));

    // Test case 3: Already sorted array
    std::vector<int> arr3 = {1, 2, 3, 4, 5};
    mergesortWrapper(arr3);
    assert((arr3 == std::vector<int>{1, 2, 3, 4, 5}));

    // Test case 4: Reverse sorted array
    std::vector<int> arr4 = {5, 4, 3, 2, 1};
    mergesortWrapper(arr4);
    assert((arr4 == std::vector<int>{1, 2, 3, 4, 5}));

    // Test case 5: Random order array
    std::vector<int> arr5 = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    mergesortWrapper(arr5);
    assert((arr5 == std::vector<int>{1, 1, 2, 3, 3, 4, 5, 5, 5, 6, 9}));

    // Test case 6: Array with duplicate elements
    std::vector<int> arr6 = {4, 2, 2, 8, 3, 3, 1};
    mergesortWrapper(arr6);
    assert((arr6 == std::vector<int>{1, 2, 2, 3, 3, 4, 8}));

    // Test case 7: Very large array
    std::vector<int> arr7(100);
    std::generate(arr7.begin(), arr7.end(), std::rand);
    mergesortWrapper(arr7);
    assert(std::is_sorted(arr7.begin(), arr7.end()));

    std::cout << "All test cases passed!" << std::endl;
}