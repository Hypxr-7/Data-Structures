#include <cassert>
#include <iostream>
#include <vector>

template<typename T>
void selectionSort(std::vector<T>& arr);

void testSelectionSort();


int main() {
    testSelectionSort();

    return 0;
}


template<typename T>
void selectionSort(std::vector<T>& arr) {
    int n = arr.size();
    for (int i = 0; i < n; ++i) {
        int min = i;
        for (int j = i + 1; j < n; ++j)
            if (arr[j] < arr[min]) 
                min = j;
        std::swap(arr[i], arr[min]);
    }
}

void testSelectionSort() {
    // Test case 1: Empty array
    std::vector<int> arr1 = {};
    selectionSort(arr1);
    assert(arr1.empty());

    // Test case 2: Single element array
    std::vector<int> arr2 = {1};
    selectionSort(arr2);
    assert(arr2 == std::vector<int>({1}));

    // Test case 3: Already sorted array
    std::vector<int> arr3 = {1, 2, 3, 4, 5};
    selectionSort(arr3);
    assert(arr3 == std::vector<int>({1, 2, 3, 4, 5}));

    // Test case 4: Reverse sorted array
    std::vector<int> arr4 = {5, 4, 3, 2, 1};
    selectionSort(arr4);
    assert(arr4 == std::vector<int>({1, 2, 3, 4, 5}));

    // Test case 5: Random order array
    std::vector<int> arr5 = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    selectionSort(arr5);
    assert(arr5 == std::vector<int>({1, 1, 2, 3, 3, 4, 5, 5, 5, 6, 9}));

    // Test case 6: Array with duplicate elements
    std::vector<int> arr6 = {2, 3, 2, 3, 1, 1};
    selectionSort(arr6);
    assert(arr6 == std::vector<int>({1, 1, 2, 2, 3, 3}));

    std::cout << "All test cases passed!" << std::endl;
}
