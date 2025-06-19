#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

#include "../data_structures/MergesortOptimized.hpp"

/**
 * @brief Test utility to check if a vector is sorted in ascending order
 */
template<typename T>
bool is_sorted(const std::vector<T>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i-1] > arr[i]) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Test utility to print vector contents for debugging
 */
template<typename T>
void print_vector(const std::vector<T>& arr, const std::string& label = "") {
    if (!label.empty()) {
        std::cout << label << ": ";
    }
    std::cout << "[";
    for (size_t i = 0; i < arr.size(); ++i) {
        std::cout << arr[i];
        if (i < arr.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

void test_empty_vector() {
    std::cout << "Testing empty vector..." << std::endl;
    std::vector<int> arr;

    try {
        MergesortOptimized<int>::sort(arr);
        assert(arr.empty());
        std::cout << "✓ Empty vector test passed" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ Empty vector test failed: " << e.what() << std::endl;
        assert(false);
    }
}

void test_single_element() {
    std::cout << "Testing single element..." << std::endl;
    std::vector<int> arr = {42};


    try {
        MergesortOptimized<int>::sort(arr);
        assert(arr.size() == 1);
        assert(arr[0] == 42);
        std::cout << "✓ Single element test passed" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ Single element test failed: " << e.what() << std::endl;
        print_vector(arr, "Result");
        assert(false);
    }
}

void test_two_elements() {
    std::cout << "Testing two elements..." << std::endl;

    // Test unsorted pair
    std::vector<int> arr1 = {5, 2};


    try {
        MergesortOptimized<int>::sort(arr1);
        print_vector(arr1, "Result");
        assert(is_sorted(arr1));
        assert(arr1[0] == 2 && arr1[1] == 5);
        std::cout << "✓ Two elements (unsorted) test passed" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ Two elements test failed: " << e.what() << std::endl;
        print_vector(arr1, "Failed result");
        assert(false);
    }

    // Test already sorted pair
    std::vector<int> arr2 = {1, 3};
    try {
        MergesortOptimized<int>::sort(arr2);
        assert(is_sorted(arr2));
        assert(arr2[0] == 1 && arr2[1] == 3);
        std::cout << "✓ Two elements (sorted) test passed" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ Two elements (sorted) test failed: " << e.what() << std::endl;
        assert(false);
    }
}

void test_already_sorted() {
    std::cout << "Testing already sorted array..." << std::endl;
    std::vector<int> arr = {1, 2, 3, 4, 5};
    const std::vector<int> expected = arr;


    try {
        print_vector(arr, "Input");
        MergesortOptimized<int>::sort(arr);
        print_vector(arr, "Result");
        assert(is_sorted(arr));
        assert(arr == expected);
        std::cout << "✓ Already sorted test passed" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ Already sorted test failed: " << e.what() << std::endl;
        print_vector(arr, "Failed result");
        assert(false);
    }
}

void test_reverse_sorted() {
    std::cout << "Testing reverse sorted array..." << std::endl;
    std::vector<int> arr = {5, 4, 3, 2, 1};
    std::vector<int> expected = {1, 2, 3, 4, 5};


    try {
        print_vector(arr, "Input");
        MergesortOptimized<int>::sort(arr);
        print_vector(arr, "Result");
        assert(is_sorted(arr));
        assert(arr == expected);
        std::cout << "✓ Reverse sorted test passed" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ Reverse sorted test failed: " << e.what() << std::endl;
        print_vector(arr, "Failed result");
        assert(false);
    }
}

void test_duplicates() {
    std::cout << "Testing array with duplicates..." << std::endl;
    std::vector<int> arr = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};


    try {
        print_vector(arr, "Input");
        MergesortOptimized<int>::sort(arr);
        print_vector(arr, "Result");
        assert(is_sorted(arr));

        // Check that all elements are still present
        std::vector<int> sorted_copy = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
        std::sort(sorted_copy.begin(), sorted_copy.end());
        assert(arr == sorted_copy);
        std::cout << "✓ Duplicates test passed" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ Duplicates test failed: " << e.what() << std::endl;
        print_vector(arr, "Failed result");
        assert(false);
    }
}

void test_random_array() {
    std::cout << "Testing random array..." << std::endl;
    std::vector<int> arr = {7, 2, 1, 6, 8, 5, 3, 4};
    std::vector<int> expected = arr;
    std::sort(expected.begin(), expected.end());



    try {
        print_vector(arr, "Input");
        MergesortOptimized<int>::sort(arr);
        print_vector(arr, "Result");
        assert(is_sorted(arr));
        assert(arr == expected);
        std::cout << "✓ Random array test passed" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ Random array test failed: " << e.what() << std::endl;
        print_vector(arr, "Failed result");
        print_vector(expected, "Expected");
        assert(false);
    }
}

void test_large_array() {
    std::cout << "Testing large array..." << std::endl;
    std::vector<int> arr;

    // Generate random data
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000);

    for (int i = 0; i < 100; ++i) {
        arr.push_back(dis(gen));
    }

    std::vector<int> expected = arr;
    std::sort(expected.begin(), expected.end());



    try {
        MergesortOptimized<int>::sort(arr);
        assert(is_sorted(arr));
        assert(arr == expected);
        std::cout << "✓ Large array test passed" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ Large array test failed: " << e.what() << std::endl;
        assert(false);
    }
}

void test_different_types() {
    std::cout << "Testing different data types..." << std::endl;

    // Test with doubles
    std::vector<double> arr_double = {3.14, 2.71, 1.41, 0.57};

    try {
        MergesortOptimized<double>::sort(arr_double);
        assert(is_sorted(arr_double));
        std::cout << "✓ Double type test passed" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ Double type test failed: " << e.what() << std::endl;
        assert(false);
    }

    // Test with characters
    std::vector<char> arr_char = {'z', 'a', 'm', 'b', 'y'};

    try {
        MergesortOptimized<char>::sort(arr_char);
        assert(is_sorted(arr_char));
        std::cout << "✓ Character type test passed" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ Character type test failed: " << e.what() << std::endl;
        assert(false);
    }
}

int main() {
    std::cout << "=== Running MergesortOptimized Tests ===" << std::endl;
    std::cout << std::endl;

    try {
        test_empty_vector();
        test_single_element();
        test_two_elements();
        test_already_sorted();
        test_reverse_sorted();
        test_duplicates();
        test_random_array();
        test_large_array();
        test_different_types();

        std::cout << std::endl;
        std::cout << "🎉 All tests passed!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << std::endl;
        std::cout << "❌ Test suite failed: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}