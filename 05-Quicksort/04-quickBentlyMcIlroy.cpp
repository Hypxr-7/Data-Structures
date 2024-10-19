/******************************************************************************
 *  Uses the Bentley-McIlroy 3-way partitioning scheme,
 *  chooses the partitioning element using Tukey's ninther,
 *  and cuts off to insertion sort.
 ******************************************************************************/

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

constexpr int INSERTION_SORT_CUTOFF = 8;
constexpr int MEDIAN_OF_3_CUTOFF = 40;

template<typename T>
void quicksortWrapper(std::vector<T>& arr);
template<typename T>
void quicksort(std::vector<T>& arr, int low, int high);

template<typename T>
void insertionSort(std::vector<T>& arr, int low, int high);
// return the index of the median element among arr[i], arr[j], and arr[k]
template<typename T>
int median3(std::vector<T>& arr, int i, int j, int k);

void testQuicksortWrapper();

int main() {
    testQuicksortWrapper();

    return 0;
}

template<typename T>
void quicksortWrapper(std::vector<T>& arr) {
    quicksort(arr, 0, arr.size() - 1);
}

template<typename T>
void quicksort(std::vector<T>& arr, int low, int high) {
    int n = high - low + 1;

    // cutoff to insertion sort
    if (n <= INSERTION_SORT_CUTOFF) {
        insertionSort(arr, low, high);
        return;
    }

    // median-of-3 as partitioning element
    else if (n <= MEDIAN_OF_3_CUTOFF) {
        int m = median3(arr, low, low + n / 2, high);
        std::swap(arr[m], arr[low]);
    }

    // use Tukey ninther as partitioning element
    else {
        int eps = n / 8;
        int mid = low + n / 2;
        int m1 = median3(arr, low, low + eps, low + eps + eps);
        int m2 = median3(arr, mid - eps, mid, mid + eps);
        int m3 = median3(arr, high - eps - eps, high - eps, high);
        int ninther = median3(arr, m1, m2, m3);
        std::swap(arr[ninther], arr[low]);
    }  

    // Bentley-McIlroy 3-way partitioning
    int i = low, j = high + 1;
    int p = low, q = high + 1;
    T v = arr[low];
    while (true) {
        while (arr[++i] < v)
            if (i == high) break;
        while (v < arr[--j])
            if (j == low) break;

        // pointers cross
        if (i == j && arr[i] == v)
            std::swap(arr[++p], arr[i]);
        if (i >= j) break;

        std::swap(arr[i], arr[j]);
        if (arr[i] == v) std::swap(arr[++p], arr[i]);
        if (arr[j] == v) std::swap(arr[--q], arr[j]);
    }

    i = j + 1;
    for (int k = low; k <= p; ++k)
        std::swap(arr[k], arr[j--]);
    for (int k = high; k >= q; --k)
        std::swap(arr[k], arr[i++]);

    quicksort(arr, low, j);
    quicksort(arr, i, high);
}

template<typename T>
void insertionSort(std::vector<T>& arr, int low, int high) {
    for (int i = low; i <= high; ++i)
        for (int j = i; j > low && arr[j] < arr[j-1]; --j)
            std::swap(arr[j], arr[j - 1]);
}

template <typename T>
int median3(std::vector<T>& arr, int i, int j, int k) {
    return ((arr[i] < arr[j]) ?
           ((arr[j] < arr[k]) ? j : (arr[i] < arr[k]) ? k : i) :
           ((arr[k] < arr[j]) ? j : (arr[k] < arr[i]) ? k : i));
}

void testQuicksortWrapper() {
    // Test case 1: Empty array
    std::vector<int> arr1 = {};
    quicksortWrapper(arr1);
    assert((arr1 == std::vector<int>{}));

    // Test case 2: Single element array
    std::vector<int> arr2 = {1};
    quicksortWrapper(arr2);
    assert((arr2 == std::vector<int>{1}));

    // Test case 3: Already sorted array
    std::vector<int> arr3 = {1, 2, 3, 4, 5};
    quicksortWrapper(arr3);
    assert((arr3 == std::vector<int>{1, 2, 3, 4, 5}));

    // Test case 4: Reverse sorted array
    std::vector<int> arr4 = {5, 4, 3, 2, 1};
    quicksortWrapper(arr4);
    assert((arr4 == std::vector<int>{1, 2, 3, 4, 5}));

    // Test case 5: Random unsorted array
    std::vector<int> arr5 = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    quicksortWrapper(arr5);
    assert((arr5 == std::vector<int>{1, 1, 2, 3, 3, 4, 5, 5, 5, 6, 9}));

    // Test case 6: Array with duplicate elements
    std::vector<int> arr6 = {4, 2, 4, 2, 4, 2};
    quicksortWrapper(arr6);
    assert((arr6 == std::vector<int>{2, 2, 2, 4, 4, 4}));

    // Test case 7: Large array
    std::vector<int> arr7(10000);
    std::iota(arr7.begin(), arr7.end(), 0);
    std::shuffle(arr7.begin(), arr7.end(), std::mt19937{std::random_device{}()});
    quicksortWrapper(arr7);
    assert(std::is_sorted(arr7.begin(), arr7.end()));

    std::cout << "All test cases passed!" << std::endl;
}
