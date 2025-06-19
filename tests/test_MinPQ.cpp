#include <algorithm>
#include <cassert>
#include <chrono>
#include <climits>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

// Include the MinPQ header
#include "../data_structures/MinPQ.hpp"

// Test helper functions
template <typename T>
std::vector<T> extractAllElements(MinPQ<T> pq) {
    std::vector<T> result;
    while (!pq.empty()) {
        result.push_back(pq.deleteMin());
    }
    return result;
}

template <typename T>
bool isSortedAscending(const std::vector<T>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] < arr[i-1]) return false;
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

    MinPQ<int> pq;

    // Test empty state
    assert(pq.empty());
    assert(pq.size() == 0);

    // Test exception on deleteMin from empty queue
    try {
        pq.deleteMin();
        assert(false);  // Should not reach here
    } catch (const std::underflow_error&) {
        // Expected exception
    }

    // Test exception on min from empty queue
    try {
        pq.min();
        assert(false);  // Should not reach here
    } catch (const std::underflow_error&) {
        // Expected exception
    }

    std::cout << "PASSED\n";
}

void test_single_element() {
    std::cout << "Testing single element operations... ";

    MinPQ<int> pq;
    pq.insert(42);

    assert(!pq.empty());
    assert(pq.size() == 1);
    assert(pq.min() == 42);

    int min_val = pq.deleteMin();
    assert(min_val == 42);
    assert(pq.empty());
    assert(pq.size() == 0);

    std::cout << "PASSED\n";
}

void test_basic_operations() {
    std::cout << "Testing basic insert/deleteMin operations... ";

    MinPQ<int> pq;

    // Insert elements in random order
    std::vector<int> elements = {10, 5, 20, 15, 30, 25};
    for (int elem : elements) {
        pq.insert(elem);
    }

    assert(pq.size() == 6);
    assert(pq.min() == 5);  // Minimum should be 5

    // Extract all elements - should come out in ascending order
    std::vector<int> extracted = extractAllElements(pq);
    std::vector<int> expected = {5, 10, 15, 20, 25, 30};

    assert(extracted == expected);
    assert(isSortedAscending(extracted));

    std::cout << "PASSED\n";
}

void test_initializer_list_constructor() {
    std::cout << "Testing initializer list constructor... ";

    MinPQ<int> pq{3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};

    assert(pq.size() == 11);
    assert(pq.min() == 1);

    // Extract all and verify sorted order
    std::vector<int> extracted = extractAllElements(pq);
    assert(isSortedAscending(extracted));
    assert(extracted[0] == 1);  // First should be minimum

    std::cout << "PASSED\n";
}

void test_vector_constructor() {
    std::cout << "Testing vector constructor... ";

    std::vector<int> input = {64, 34, 25, 12, 22, 11, 90};
    MinPQ<int> pq(input);

    assert(pq.size() == 7);
    assert(pq.min() == 11);

    std::vector<int> extracted = extractAllElements(pq);
    assert(isSortedAscending(extracted));
    assert(extracted[0] == 11);

    std::cout << "PASSED\n";
}

void test_duplicates() {
    std::cout << "Testing duplicate elements... ";

    MinPQ<int> pq;

    // Insert many duplicates
    for (int i = 0; i < 5; ++i) {
        pq.insert(10);
        pq.insert(20);
        pq.insert(10);
    }

    assert(pq.size() == 15);
    assert(pq.min() == 10);

    // Should extract all 10s first, then all 20s
    std::vector<int> extracted = extractAllElements(pq);
    assert(isSortedAscending(extracted));

    // Count elements
    int count_10 = std::count(extracted.begin(), extracted.end(), 10);
    int count_20 = std::count(extracted.begin(), extracted.end(), 20);
    assert(count_10 == 10);
    assert(count_20 == 5);

    std::cout << "PASSED\n";
}

void test_different_types() {
    std::cout << "Testing different data types... ";

    // Test with strings
    MinPQ<std::string> string_pq;
    string_pq.insert("zebra");
    string_pq.insert("apple");
    string_pq.insert("banana");
    string_pq.insert("orange");

    assert(string_pq.min() == "apple");  // Lexicographically smallest

    std::vector<std::string> string_extracted = extractAllElements(string_pq);
    assert(isSortedAscending(string_extracted));

    // Test with doubles
    MinPQ<double> double_pq{3.14, 2.71, 1.41, 1.73, 2.23};
    assert(double_pq.min() == 1.41);

    std::vector<double> double_extracted = extractAllElements(double_pq);
    assert(isSortedAscending(double_extracted));

    std::cout << "PASSED\n";
}

void test_large_dataset() {
    std::cout << "Testing large dataset (10000 elements)... ";

    auto large_data = generateRandomVector<int>(10000, 1, 100000);
    int expected_min = *std::min_element(large_data.begin(), large_data.end());

    auto start = std::chrono::high_resolution_clock::now();

    MinPQ<int> pq(large_data);
    assert(pq.size() == 10000);
    assert(pq.min() == expected_min);

    // Extract first 100 elements to verify order
    std::vector<int> first_100;
    for (int i = 0; i < 100 && !pq.empty(); ++i) {
        first_100.push_back(pq.deleteMin());
    }
    assert(isSortedAscending(first_100));

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "PASSED (took " << duration.count() << "ms)\n";
}

void test_dynamic_resizing() {
    std::cout << "Testing dynamic resizing... ";

    MinPQ<int> pq;

    // Insert many elements to force multiple resizes
    for (int i = 1000; i >= 1; --i) {
        pq.insert(i);
        assert(pq.min() == i);  // Minimum should always be the latest inserted
    }

    assert(pq.size() == 1000);

    // Delete many elements to force shrinking
    for (int i = 1; i <= 1000; ++i) {
        int min_val = pq.deleteMin();
        assert(min_val == i);
    }

    assert(pq.empty());

    std::cout << "PASSED\n";
}

void test_mixed_operations() {
    std::cout << "Testing mixed insert/delete operations... ";

    MinPQ<int> pq;

    // Interleave insertions and deletions
    pq.insert(10);
    pq.insert(20);
    assert(pq.deleteMin() == 10);

    pq.insert(15);
    pq.insert(25);
    pq.insert(5);
    assert(pq.deleteMin() == 5);
    assert(pq.deleteMin() == 15);

    pq.insert(30);
    assert(pq.min() == 20);
    assert(pq.size() == 3);  // Should have 20, 25, 30

    std::vector<int> remaining = extractAllElements(pq);
    std::vector<int> expected = {20, 25, 30};
    assert(remaining == expected);

    std::cout << "PASSED\n";
}

void test_clear_operation() {
    std::cout << "Testing clear operation... ";

    MinPQ<int> pq{1, 2, 3, 4, 5};
    assert(pq.size() == 5);
    assert(!pq.empty());

    pq.clear();
    assert(pq.size() == 0);
    assert(pq.empty());

    // Should be able to use after clearing
    pq.insert(42);
    assert(pq.size() == 1);
    assert(pq.min() == 42);

    std::cout << "PASSED\n";
}

void test_heap_property_maintenance() {
    std::cout << "Testing heap property maintenance... ";

    MinPQ<int> pq;
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
            // Delete min
            pq.deleteMin();
        }

        // Verify min is actually minimum (if not empty)
        if (!pq.empty()) {
            int current_min = pq.min();
            auto elements = pq.getElements();
            int actual_min = *std::min_element(elements.begin(), elements.end());
            assert(current_min == actual_min);
        }
    }

    std::cout << "PASSED\n";
}

void test_custom_type() {
    std::cout << "Testing custom type... ";

    struct Task {
        std::string name;
        int priority;  // Lower number = higher priority

        // Default constructor
        Task() : name(""), priority(0) {}

        Task(const std::string& n, int p) : name(n), priority(p) {}

        bool operator<(const Task& other) const {
            return priority < other.priority;
        }

        bool operator>(const Task& other) const {
            return priority > other.priority;
        }

        bool operator<=(const Task& other) const {
            return priority <= other.priority;
        }

        bool operator==(const Task& other) const {
            return name == other.name && priority == other.priority;
        }
    };

    MinPQ<Task> pq;
    pq.insert(Task("Low Priority", 10));
    pq.insert(Task("High Priority", 1));
    pq.insert(Task("Medium Priority", 5));

    assert(pq.min().name == "High Priority");  // Lowest priority number = highest priority
    assert(pq.deleteMin().name == "High Priority");
    assert(pq.deleteMin().name == "Medium Priority");
    assert(pq.deleteMin().name == "Low Priority");

    std::cout << "PASSED\n";
}

void test_dijkstra_simulation() {
    std::cout << "Testing Dijkstra-like simulation... ";

    struct Edge {
        int vertex;
        double weight;

        Edge() : vertex(0), weight(0.0) {}
        Edge(int v, double w) : vertex(v), weight(w) {}

        bool operator<(const Edge& other) const {
            return weight < other.weight;
        }

        bool operator>(const Edge& other) const {
            return weight > other.weight;
        }

        bool operator<=(const Edge& other) const {
            return weight <= other.weight;
        }
    };

    MinPQ<Edge> edgePQ;

    // Simulate adding edges with different weights
    edgePQ.insert(Edge(1, 4.5));
    edgePQ.insert(Edge(2, 2.1));
    edgePQ.insert(Edge(3, 7.8));
    edgePQ.insert(Edge(4, 1.2));
    edgePQ.insert(Edge(5, 3.4));

    // Should extract edges in order of increasing weight
    std::vector<double> weights;
    while (!edgePQ.empty()) {
        weights.push_back(edgePQ.deleteMin().weight);
    }

    assert(isSortedAscending(weights));
    assert(weights[0] == 1.2);  // Shortest edge first

    std::cout << "PASSED\n";
}

void test_k_smallest_elements() {
    std::cout << "Testing k-smallest elements extraction... ";

    std::vector<int> data = {64, 34, 25, 12, 22, 11, 90, 88, 76, 50, 42};
    MinPQ<int> pq(data);

    // Extract 5 smallest elements
    std::vector<int> smallest_5;
    for (int i = 0; i < 5 && !pq.empty(); ++i) {
        smallest_5.push_back(pq.deleteMin());
    }

    std::vector<int> expected = {11, 12, 22, 25, 34};
    assert(smallest_5 == expected);
    assert(isSortedAscending(smallest_5));

    std::cout << "PASSED\n";
}

void performance_comparison() {
    std::cout << "\n=== Performance Comparison ===\n";

    const int size = 100000;
    auto data = generateRandomVector<int>(size, 1, size);

    // Test MinPQ construction and operations
    auto start = std::chrono::high_resolution_clock::now();
    MinPQ<int> pq(data);
    auto construction_time = std::chrono::high_resolution_clock::now();

    // Extract all elements (heap sort ascending)
    std::vector<int> sorted_asc;
    while (!pq.empty()) {
        sorted_asc.push_back(pq.deleteMin());
    }
    auto extraction_time = std::chrono::high_resolution_clock::now();

    auto construct_duration = std::chrono::duration_cast<std::chrono::milliseconds>(construction_time - start);
    auto extract_duration = std::chrono::duration_cast<std::chrono::milliseconds>(extraction_time - construction_time);

    std::cout << "MinPQ construction (" << size << " elements): " << construct_duration.count() << "ms\n";
    std::cout << "MinPQ extraction (heap sort): " << extract_duration.count() << "ms\n";

    // Verify it's actually sorted
    assert(isSortedAscending(sorted_asc));

    // Compare with std::sort
    auto data_copy = data;
    start = std::chrono::high_resolution_clock::now();
    std::sort(data_copy.begin(), data_copy.end());
    auto std_sort_time = std::chrono::high_resolution_clock::now();
    auto std_duration = std::chrono::duration_cast<std::chrono::milliseconds>(std_sort_time - start);

    std::cout << "std::sort (ascending): " << std_duration.count() << "ms\n";

    // Results should be the same
    assert(sorted_asc == data_copy);

    // Test finding k smallest elements efficiently
    auto large_data = generateRandomVector<int>(1000000, 1, 1000000);
    const int k = 100;

    start = std::chrono::high_resolution_clock::now();
    MinPQ<int> large_pq(large_data);
    std::vector<int> k_smallest;
    for (int i = 0; i < k && !large_pq.empty(); ++i) {
        k_smallest.push_back(large_pq.deleteMin());
    }
    auto k_smallest_time = std::chrono::high_resolution_clock::now();
    auto k_duration = std::chrono::duration_cast<std::chrono::milliseconds>(k_smallest_time - start);

    std::cout << "Find " << k << " smallest from " << large_data.size() << " elements: " << k_duration.count() << "ms\n";
    assert(isSortedAscending(k_smallest));
}

void test_edge_cases() {
    std::cout << "Testing edge cases... ";

    // Test with minimum and maximum values
    MinPQ<int> pq;
    pq.insert(INT_MAX);
    pq.insert(INT_MIN);
    pq.insert(0);

    assert(pq.deleteMin() == INT_MIN);
    assert(pq.deleteMin() == 0);
    assert(pq.deleteMin() == INT_MAX);

    // Test rapid insertions and deletions
    for (int i = 100; i >= 1; --i) {
        pq.insert(i);
        if (i % 3 == 0 && !pq.empty()) {
            pq.deleteMin();
        }
    }

    // Should still maintain heap property
    std::vector<int> remaining = extractAllElements(pq);
    assert(isSortedAscending(remaining));

    // Test with negative numbers
    MinPQ<int> neg_pq;
    neg_pq.insert(-5);
    neg_pq.insert(-1);
    neg_pq.insert(-10);
    neg_pq.insert(0);
    neg_pq.insert(3);

    std::vector<int> neg_extracted = extractAllElements(neg_pq);
    std::vector<int> neg_expected = {-10, -5, -1, 0, 3};
    assert(neg_extracted == neg_expected);

    std::cout << "PASSED\n";
}

void test_min_max_comparison() {
    std::cout << "Testing MinPQ vs conceptual MaxPQ behavior... ";

    std::vector<int> data = {5, 2, 8, 1, 9, 3};

    // MinPQ should extract in ascending order
    MinPQ<int> min_pq(data);
    std::vector<int> min_extracted = extractAllElements(min_pq);

    // Verify ascending order
    assert(isSortedAscending(min_extracted));
    assert(min_extracted == std::vector<int>({1, 2, 3, 5, 8, 9}));

    std::cout << "PASSED\n";
}

int main() {
    std::cout << "=== MinPQ Priority Queue Test Suite ===\n\n";

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
        test_dijkstra_simulation();
        test_k_smallest_elements();
        test_edge_cases();
        test_min_max_comparison();

        std::cout << "\n=== All tests PASSED! ===\n";

        performance_comparison();

    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}