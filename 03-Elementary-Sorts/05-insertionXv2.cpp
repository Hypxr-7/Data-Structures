#include <iostream>
#include <vector>

template<typename T>
void insertionSort(std::vector<T>& arr);

void testInsertionSort();

int main() {
    testInsertionSort();
    return 0;
}

template<typename T>
void insertionSort(std::vector<T>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        T key = arr[i];
        int j = i - 1;

        // Move elements of arr[0..i-1], that are greater than key,
        // to one position ahead of their current position
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

void testInsertionSort() {
    // Test case 1: Empty array
    std::vector<int> arr1 = {};
    insertionSort(arr1);
    std::cout << "Test case 1: ";
    for (const auto& elem : arr1) std::cout << elem << " ";
    std::cout << std::endl;

    // Test case 2: Single element array
    std::vector<int> arr2 = {1};
    insertionSort(arr2);
    std::cout << "Test case 2: ";
    for (const auto& elem : arr2) std::cout << elem << " ";
    std::cout << std::endl;

    // Test case 3: Already sorted array
    std::vector<int> arr3 = {1, 2, 3, 4, 5};
    insertionSort(arr3);
    std::cout << "Test case 3: ";
    for (const auto& elem : arr3) std::cout << elem << " ";
    std::cout << std::endl;

    // Test case 4: Reverse sorted array
    std::vector<int> arr4 = {5, 4, 3, 2, 1};
    insertionSort(arr4);
    std::cout << "Test case 4: ";
    for (const auto& elem : arr4) std::cout << elem << " ";
    std::cout << std::endl;

    // Test case 5: Random array
    std::vector<int> arr5 = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    insertionSort(arr5);
    std::cout << "Test case 5: ";
    for (const auto& elem : arr5) std::cout << elem << " ";
    std::cout << std::endl;
}