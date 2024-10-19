#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

constexpr int CUTOFF = 10;

template<typename T>
void merge(std::vector<T>& src, std::vector<T>& dst, int low, int mid, int high);
template<typename T>
void mergesort(std::vector<T>& src, std::vector<T>& dst, int low, int high);
template<typename T>
void mergesortWrapper(std::vector<T>& arr);

template<typename T>
void insertionSort(std::vector<T>& arr, int low, int high);

void testMergesortWrapper();

int main() {
    testMergesortWrapper();

    return 0;
}

template<typename T>
void merge(std::vector<T>& src, std::vector<T>& dst, int low, int mid, int high) {

    // improvement 3: removed the need to copy the array 

    int i = low, j = mid + 1;
    for (int k = low; k <= high; ++k) {
        if      (i > mid)          dst[k] = src[j++];
        else if (j > high)         dst[k] = src[i++];
        else if (src[j] < src[i])  dst[k] = src[j++];   
        else                       dst[k] = src[i++];
    }
}

template<typename T>
void mergesort(std::vector<T>& src, std::vector<T>& dst, int low, int high) {
    // improvement 1: adds cutoff and uses insertion sort for smaller sub-arrays
    if (high <= low + CUTOFF) {
        insertionSort(dst, low, high);
        return;
    }

    int mid = low + (high - low) / 2;
    mergesort(dst, src, low, mid);
    mergesort(dst, src, mid + 1, high);

    // improvement 2: skips recursive call if already sorted
    if (src[mid + 1] >= src[mid]) return;

    merge(src, dst, low, mid, high);
}

template<typename T>
void mergesortWrapper(std::vector<T>& arr) {
    std::vector<T> aux(arr);
    mergesort(aux, arr, 0, arr.size() - 1);
}

template<typename T>
void insertionSort(std::vector<T>& arr, int low, int high) {
    for (int i = low; i <= high; ++i) {
        for (int j = i; j > low && arr[j] < arr[j - 1]; --j) {
            std::swap(arr[j], arr[j - 1]);
        }
    }
}

void testMergesortWrapper() {
    // Test case 1: Empty array
    std::vector<int> arr1;
    mergesortWrapper(arr1);
    assert(arr1.empty());

    // Test case 2: Single element array
    std::vector<int> arr2 = {1};
    mergesortWrapper(arr2);
    assert(arr2 == std::vector<int>({1}));

    // Test case 3: Already sorted array
    std::vector<int> arr3 = {1, 2, 3, 4, 5};
    mergesortWrapper(arr3);
    assert(arr3 == std::vector<int>({1, 2, 3, 4, 5}));

    // Test case 4: Reverse sorted array
    std::vector<int> arr4 = {5, 4, 3, 2, 1};
    mergesortWrapper(arr4);
    assert(arr4 == std::vector<int>({1, 2, 3, 4, 5}));

    // Test case 5: Random order array
    std::vector<int> arr5 = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    mergesortWrapper(arr5);
    assert(arr5 == std::vector<int>({1, 1, 2, 3, 3, 4, 5, 5, 5, 6, 9}));

    // Test case 6: Array with duplicates
    std::vector<int> arr6 = {2, 3, 2, 3, 2, 3};
    mergesortWrapper(arr6);
    assert(arr6 == std::vector<int>({2, 2, 2, 3, 3, 3}));

    // Test case 7: Very large array
    std::vector<int> arr7(100);
    std::generate(arr7.begin(), arr7.end(), std::rand);
    mergesortWrapper(arr7);
    assert(std::is_sorted(arr7.begin(), arr7.end()));

    std::cout << "All test cases passed!" << std::endl;
}