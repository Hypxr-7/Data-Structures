/******************************************************************************
 *  Generate a best-case input of size n for standard quicksort.
 ******************************************************************************/

#include <cassert>
#include <iostream>
#include <vector>

template<typename T>
void best(std::vector<T>& arr, int low, int high);
template<typename T>
std::vector<T> bestWrapper(int n);

void testBestWrapper();

int main() {
    testBestWrapper();

    return 0;
}

template <typename T>
void best(std::vector<T>& arr, int low, int high) {
    // precondition:  arr[low..high] contains keys lo to hi, in order
    for (int i = low; i <= high; i++)
        assert(arr[i] == i);

    if (high <= low) return;

    int mid = low + (high - low) / 2;
    best(arr, low, mid-1);
    best(arr, mid+1, high);
    std::swap(arr[low], arr[mid]);
}

template <typename T>
std::vector<T> bestWrapper(int n) {
    std::vector<T> arr;

    for (int i = 0; i < n; ++i)
        arr.push_back(i);

    best(arr, 0, n - 1);

    return arr;
}

void testBestWrapper() {
    // Test case 1: n = 1
    std::vector<int> result1 = bestWrapper<int>(1);
    assert(result1.size() == 1);
    assert(result1[0] == 0);

    // Test case 2: n = 2
    std::vector<int> result2 = bestWrapper<int>(2);
    assert(result2.size() == 2);
    assert(result2[0] == 0);
    assert(result2[1] == 1);

    // Test case 3: n = 3
    std::vector<int> result3 = bestWrapper<int>(3);
    assert(result3.size() == 3);
    assert(result3[0] == 1);
    assert(result3[1] == 0);
    assert(result3[2] == 2);

    // Test case 4: n = 7
    std::vector<int> result4 = bestWrapper<int>(7);
    assert(result4.size() == 7);
    assert(result4[0] == 3);
    assert(result4[1] == 0);
    assert(result4[2] == 2);
    assert(result4[3] == 1);
    assert(result4[4] == 5);
    assert(result4[5] == 4);
    assert(result4[6] == 6);

    std::cout << "All test cases passed!" << std::endl;
}
