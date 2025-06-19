#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

// Include the MaxPQ header
#include "../data_structures/MaxPQ.hpp"

// Test helper functions
template <typename T>
std::vector<T> extractAllElements(MaxPQ<T> pq) {
    std::vector<T> result;
    while (!pq.empty()) {
        result.push_back(pq.deleteMax());
    }
    return result;
}

template <typename T>
bool isSortedDescending(const std::vector<T>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] > arr[i-1]) return false;
    }
    return true;
}

template <typename T>
std::vector<T> generateRandomVector(size_t size, T min_val, T max_val) {
    std::vector<T> vec(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<T> dis(min_val, max_val);

    for (auto& elem : vec) {
        elem = dis(gen);
    }
    return vec;
}

// Test Functions
void test_empty_queue() {
    std::cout << "Testing empty queue operations... ";

    MaxPQ<int> pq;

    // Test empty state
    assert(pq.empty());
    assert(pq.size() == 0);

    // Test exception on deleteMax from empty queue
    try {
        pq.deleteMax();
        assert(false);  // Should not reach here
    } catch (const std::underflow_error&) {
        // Expected exception
    }

    // Test exception on max from empty queue
    try {
        pq.max();
        assert(false);  // Should not reach here
    } catch (const std::underflow_error&) {
        // Expected exception
    }

    std::cout << "PASSED\n";
}

void test_single_element() {
    std::cout << "Testing single element operations... ";

    MaxPQ<int> pq;
    pq.insert(42);

    assert(!pq.empty());
    assert(pq.size() == 1);
    assert(pq.max() == 42);

    int max_val = pq.deleteMax();
    assert(max_val == 42);
    assert(pq.empty());
    assert(pq.size() == 0);

    std::cout << "PASSED\n";
}

void test_basic_operations() {
    std::cout << "Testing basic insert/deleteMax operations... ";

    MaxPQ<int> pq;

    // Insert elements in random order
    std::vector<int> elements = {10, 5, 20, 15, 30, 25};
    for (int elem : elements) {
        pq.insert(elem);
    }

    assert(pq.size() == 6);
    assert(pq.max() == 30);  // Maximum should be 30

    // Extract all elements - should come out in descending order
    std::vector<int> extracted = extractAllElements(pq);
    std::vector<int> expected = {30, 25, 20, 15, 10, 5};

    assert(extracted == expected);
    assert(isSortedDescending(extracted));

    std::cout << "PASSED\n";
}

void test_initializer_list_constructor() {
    std::cout << "Testing initializer list constructor... ";

    MaxPQ<int> pq{3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};

    assert(pq.size() == 11);
    assert(pq.max() == 9);

    // Extract all and verify sorted order
    std::vector<int> extracted = extractAllElements(pq);
    assert(isSortedDescending(extracted));
    assert(extracted[0] == 9);  // First should be maximum

    std::cout << "PASSED\n";
}

void test_vector_constructor() {
    std::cout << "Testing vector constructor... ";

    std::vector<int> input = {64, 34, 25, 12, 22, 11, 90};
    MaxPQ<int> pq(input);

    assert(pq.size() == 7);
    assert(pq.max() == 90);

    std::vector<int> extracted = extractAllElements(pq);
    assert(isSortedDescending(extracted));
    assert(extracted[0] == 90);

    std::cout << "PASSED\n";
}

void test_duplicates() {
    std::cout << "Testing duplicate elements... ";

    MaxPQ<int> pq;

    // Insert many duplicates
    for (int i = 0; i < 5; ++i) {
        pq.insert(10);
        pq.insert(20);
        pq.insert(10);
    }

    assert(pq.size() == 15);
    assert(pq.max() == 20);

    // Should extract all 20s first, then all 10s
    std::vector<int> extracted = extractAllElements(pq);
    assert(isSortedDescending(extracted));

    // Count elements
    int count_20 = std::count(extracted.begin(), extracted.end(), 20);
    int count_10 = std::count(extracted.begin(), extracted.end(), 10);
    assert(count_20 == 5);
    assert(count_10 == 10);

    std::cout << "PASSED\n";
}

void test_different_types() {
    std::cout << "Testing different data types... ";

    // Test with strings
    MaxPQ<std::string> string_pq;
    string_pq.insert("zebra");
    string_pq.insert("apple");
    string_pq.insert("banana");
    string_pq.insert("orange");

    assert(string_pq.max() == "zebra");  // Lexicographically largest

    std::vector<std::string> string_extracted = extractAllElements(string_pq);
    assert(isSortedDescending(string_extracted));

    // Test with doubles
    MaxPQ<double> double_pq{3.14, 2.71, 1.41, 1.73, 2.23};
    assert(double_pq.max() == 3.14);

    std::vector<double> double_extracted = extractAllElements(double_pq);
    assert(isSortedDescending(double_extracted));

    std::cout << "PASSED\n";
}

void test_large_dataset() {
    std::cout << "Testing large dataset (10000 elements)... ";

    auto large_data = generateRandomVector<int>(10000, 1, 100000);
    int expected_max = *std::max_element(large_data.begin(), large_data.end());

    auto start = std::chrono::high_resolution_clock::now();

    MaxPQ<int> pq(large_data);
    assert(pq.size() == 10000);
    assert(pq.max() == expected_max);

    // Extract first 100 elements to verify order
    std::vector<int> first_100;
    for (int i = 0; i < 100 && !pq.empty(); ++i) {
        first_100.push_back(pq.deleteMax());
    }
    assert(isSortedDescending(first_100));

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "PASSED (took " << duration.count() << "ms)\n";
}

void test_dynamic_resizing() {
    std::cout << "Testing dynamic resizing... ";

    MaxPQ<int> pq;

    // Insert many elements to force multiple resizes
    for (int i = 1; i <= 1000; ++i) {
        pq.insert(i);
        assert(pq.max() == i);  // Maximum should always be the latest inserted
    }

    assert(pq.size() == 1000);

    // Delete many elements to force shrinking
    for (int i = 1000; i >= 1; --i) {
        int max_val = pq.deleteMax();
        assert(max_val == i);
    }

    assert(pq.empty());

    std::cout << "PASSED\n";
}

void test_mixed_operations() {
    std::cout << "Testing mixed insert/delete operations... ";

    MaxPQ<int> pq;

    // Interleave insertions and deletions
    pq.insert(10);
    pq.insert(20);
    assert(pq.deleteMax() == 20);

    pq.insert(15);
    pq.insert(25);
    pq.insert(5);
    assert(pq.deleteMax() == 25);
    assert(pq.deleteMax() == 15);

    pq.insert(30);
    assert(pq.max() == 30);
    assert(pq.size() == 3);  // Should have 30, 10, 5

    std::vector<int> remaining = extractAllElements(pq);
    std::vector<int> expected = {30, 10, 5};
    assert(remaining == expected);

    std::cout << "PASSED\n";
}

void test_clear_operation() {
    std::cout << "Testing clear operation... ";

    MaxPQ<int> pq{1, 2, 3, 4, 5};
    assert(pq.size() == 5);
    assert(!pq.empty());

    pq.clear();
    assert(pq.size() == 0);
    assert(pq.empty());

    // Should be able to use after clearing
    pq.insert(42);
    assert(pq.size() == 1);
    assert(pq.max() == 42);

    std::cout << "PASSED\n";
}

void test_heap_property_maintenance() {
    std::cout << "Testing heap property maintenance... ";

    MaxPQ<int> pq;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(1, 1000);

    // Perform many random operations
    for (int i = 0; i < 500; ++i) {
        if (pq.empty() || dis(gen) % 2 == 0) {
            // Insert
            int value = dis(gen);
            pq.insert(value);
        } else {
            // Delete max
            pq.deleteMax();
        }

        // Verify max is actually maximum (if not empty)
        if (!pq.empty()) {
            int current_max = pq.max();
            auto elements = pq.getElements();
            int actual_max = *std::max_element(elements.begin(), elements.end());
            assert(current_max == actual_max);
        }
    }

    std::cout << "PASSED\n";
}

void test_custom_type() {
    std::cout << "Testing custom type... ";

    struct Person {
        std::string name;
        int priority;

        Person() : name(""), priority(1) {}

        Person(const std::string& n, int p) : name(n), priority(p) {}

        bool operator<(const Person& other) const {
            return priority < other.priority;
        }

        bool operator>(const Person& other) const {
            return priority > other.priority;
        }

        bool operator>=(const Person& other) const {
            return priority >= other.priority;
        }

        bool operator==(const Person& other) const {
            return name == other.name && priority == other.priority;
        }
    };

    MaxPQ<Person> pq;
    pq.insert(Person("Alice", 10));
    pq.insert(Person("Bob", 20));
    pq.insert(Person("Charlie", 15));

    assert(pq.max().name == "Bob");  // Highest priority
    assert(pq.deleteMax().name == "Bob");
    assert(pq.deleteMax().name == "Charlie");
    assert(pq.deleteMax().name == "Alice");

    std::cout << "PASSED\n";
}

void performance_comparison() {
    std::cout << "\n=== Performance Comparison ===\n";

    const int size = 100000;
    auto data = generateRandomVector<int>(size, 1, size);

    // Test MaxPQ construction and operations
    auto start = std::chrono::high_resolution_clock::now();
    MaxPQ<int> pq(data);
    auto construction_time = std::chrono::high_resolution_clock::now();

    // Extract all elements
    std::vector<int> sorted_desc;
    while (!pq.empty()) {
        sorted_desc.push_back(pq.deleteMax());
    }
    auto extraction_time = std::chrono::high_resolution_clock::now();

    auto construct_duration = std::chrono::duration_cast<std::chrono::milliseconds>(construction_time - start);
    auto extract_duration = std::chrono::duration_cast<std::chrono::milliseconds>(extraction_time - construction_time);

    std::cout << "MaxPQ construction (" << size << " elements): " << construct_duration.count() << "ms\n";
    std::cout << "MaxPQ extraction (heap sort): " << extract_duration.count() << "ms\n";

    // Verify it's actually sorted
    assert(isSortedDescending(sorted_desc));

    // Compare with std::sort
    auto data_copy = data;
    start = std::chrono::high_resolution_clock::now();
    std::sort(data_copy.begin(), data_copy.end(), std::greater<int>());
    auto std_sort_time = std::chrono::high_resolution_clock::now();
    auto std_duration = std::chrono::duration_cast<std::chrono::milliseconds>(std_sort_time - start);

    std::cout << "std::sort (descending): " << std_duration.count() << "ms\n";

    // Results should be the same
    assert(sorted_desc == data_copy);
}

int main() {
    std::cout << "=== MaxPQ Priority Queue Test Suite ===\n\n";

    try {
        test_empty_queue();
        test_single_element();
        test_basic_operations();
        test_initializer_list_constructor();
        test_vector_constructor();
        test_duplicates();
        test_different_types();
        test_large_dataset();
        test_dynamic_resizing();
        test_mixed_operations();
        test_clear_operation();
        test_heap_property_maintenance();
        test_custom_type();

        std::cout << "\n=== All tests PASSED! ===\n";

        performance_comparison();

    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}