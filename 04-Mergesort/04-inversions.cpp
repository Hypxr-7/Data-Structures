#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

template<typename T>
int merge(std::vector<T>& arr, std::vector<T>& aux, int low, int mid, int high);
template<typename T>
int mergesort(std::vector<T>& arr, std::vector<T>& aux, int low, int high);
template<typename T>
int mergesortWrapper(std::vector<T>& arr);

void testInversions();

int main() {
    testInversions();

    return 0;
}

template<typename T>
int merge(std::vector<T>& arr, std::vector<T>& aux, int low, int mid, int high) {
    int inversions = 0;

    for (int k = low; k <= high; ++k)
        aux[k] = arr[k];

    // if aux[j] < aux[i] elements from arr[i] to arr[mid] will form inversions with arr[j]
    int i = low, j = mid + 1;
    for (int k = low; k <= high; ++k) {
        if      (i > mid)         arr[k] = aux[j++];
        else if (j > high)        arr[k] = aux[i++];
        else if (aux[j] < aux[i]) {arr[k] = aux[j++]; inversions += (mid - i + 1);}
        else                      arr[k] = aux[i++];
    }

    return inversions;
}

template<typename T>
int mergesort(std::vector<T>& arr, std::vector<T>& aux, int low, int high) {
    int inversions = 0;
    if (high <= low) return 0;

    int mid = low + (high - low) / 2;
    inversions += mergesort(arr, aux, low, mid);
    inversions += mergesort(arr, aux, mid + 1, high);
    inversions += merge(arr, aux, low, mid, high);

    return inversions;
}

template<typename T>
int mergesortWrapper(std::vector<T>& arr) {
    std::vector<T> aux(arr.size());
    return mergesort(arr, aux, 0, arr.size() - 1);
}

void testInversions() {
    std::vector<int> arr1 = {1, 3, 5, 2, 4, 6};
    assert(mergesortWrapper(arr1) == 3);
    std::cout << "Test 1 passed!" << std::endl;

    std::vector<int> arr2 = {1, 20, 6, 4, 5};
    assert(mergesortWrapper(arr2) == 5);
    std::cout << "Test 2 passed!" << std::endl;

    std::vector<int> arr3 = {1, 2, 3, 4, 5};
    assert(mergesortWrapper(arr3) == 0);
    std::cout << "Test 3 passed!" << std::endl;

    std::vector<int> arr4 = {5, 4, 3, 2, 1};
    assert(mergesortWrapper(arr4) == 10);
    std::cout << "Test 4 passed!" << std::endl;

    std::vector<int> arr5 = {1, 1, 1, 1, 1};
    assert(mergesortWrapper(arr5) == 0);
    std::cout << "Test 5 passed!" << std::endl;
}