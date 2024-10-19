#include <cassert>
#include <iostream>
#include <vector>

#include <cassert>
#include <iostream>
#include <vector>

template<typename T>
void shellSort(std::vector<T>& arr);

void testShellSort();

int main() {
    testShellSort();

    return 0;
}


template<typename T>
void shellSort(std::vector<T>& arr) {
    int n = arr.size();

    // increments : 3x+1
    int h = 1;
    while (h < n / 3) h = 3 * h + 1;

    while (h >= 1) {
        for (int i = h; i < n; ++i) 
            for (int j = i; j >= h && arr[j] < arr[j-h]; j -= h) 
                std::swap(arr[j], arr[j-h]);
        h /= 3;             
    }
}

void testShellSort() {
    // Test case 1: Empty array
    std::vector<int> arr1 = {};
    shellSort(arr1);
    assert(arr1.empty());

    // Test case 2: Single element array
    std::vector<int> arr2 = {1};
    shellSort(arr2);
    assert(arr2 == std::vector<int>({1}));

    // Test case 3: Already sorted array
    std::vector<int> arr3 = {1, 2, 3, 4, 5};
    shellSort(arr3);
    assert(arr3 == std::vector<int>({1, 2, 3, 4, 5}));

    // Test case 4: Reverse sorted array
    std::vector<int> arr4 = {5, 4, 3, 2, 1};
    shellSort(arr4);
    assert(arr4 == std::vector<int>({1, 2, 3, 4, 5}));

    // Test case 5: Random order array
    std::vector<int> arr5 = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    shellSort(arr5);
    assert(arr5 == std::vector<int>({1, 1, 2, 3, 3, 4, 5, 5, 5, 6, 9}));

    // Test case 6: Array with duplicate elements
    std::vector<int> arr6 = {2, 3, 2, 3, 1, 1};
    shellSort(arr6);
    assert(arr6 == std::vector<int>({1, 1, 2, 2, 3, 3}));

    std::cout << "All test cases passed!" << std::endl;
}
