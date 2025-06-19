#pragma once

#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include "../data_structures/RedBlackBST.hpp"

/**
 * @brief Comprehensive test suite for Red-Black BST
 *
 * This test suite validates the Red-Black BST implementation with focus on:
 * - Red-Black tree properties (balance, color constraints)
 * - BST correctness (ordering, search operations)
 * - Performance guarantees (logarithmic height)
 * - Stress testing with large datasets
 * - Edge cases and error conditions
 */

// Helper function to test if exception is thrown
template<typename Func>
bool throws_exception(Func func) {
    try {
        func();
        return false;
    } catch (...) {
        return true;
    }
}

// Helper function to test specific exception type
template<typename ExceptionType, typename Func>
bool throws_specific_exception(Func func) {
    try {
        func();
        return false;
    } catch (const ExceptionType&) {
        return true;
    } catch (...) {
        return false;
    }
}

// Helper function to check if vector contains all expected elements in order
template<typename T>
bool vectors_equal(const std::vector<T>& vec1, const std::vector<T>& vec2) {
    if (vec1.size() != vec2.size()) return false;
    for (size_t i = 0; i < vec1.size(); ++i) {
        if (vec1[i] != vec2[i]) return false;
    }
    return true;
}

// Helper function to check if a vector is sorted
template<typename T>
bool is_sorted(const std::vector<T>& vec) {
    for (size_t i = 1; i < vec.size(); ++i) {
        if (vec[i] < vec[i-1]) return false;
    }
    return true;
}

// Helper function to generate random integers
std::vector<int> generate_random_integers(int count, int min_val = 0, int max_val = 10000) {
    std::vector<int> result;
    std::mt19937 gen(42);  // Fixed seed for reproducibility
    std::uniform_int_distribution<> dis(min_val, max_val);

    for (int i = 0; i < count; ++i) {
        result.push_back(dis(gen));
    }
    return result;
}

// =============================================================================
// Constructor and Basic State Tests
// =============================================================================

void test_default_constructor() {
    std::cout << "Testing default constructor..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;

    assert(rb_tree.empty());
    assert(rb_tree.size() == 0);

    std::cout << "✓ Default constructor test passed" << std::endl;
}

void test_destructor() {
    std::cout << "Testing destructor..." << std::endl;

    // Test with scope to trigger destructor
    {
        RedBlackBST<int, std::string> rb_tree;
        for (int i = 1; i <= 100; ++i) {
            rb_tree.put(i, std::to_string(i));
        }
        assert(rb_tree.size() == 100);
    } // Destructor should be called here

    std::cout << "✓ Destructor test passed" << std::endl;
}

void test_empty_and_size() {
    std::cout << "Testing empty() and size() methods..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;

    // Initially empty
    assert(rb_tree.empty());
    assert(rb_tree.size() == 0);

    // After adding elements
    rb_tree.put(10, "ten");
    assert(!rb_tree.empty());
    assert(rb_tree.size() == 1);

    rb_tree.put(5, "five");
    rb_tree.put(15, "fifteen");
    assert(rb_tree.size() == 3);

    // After removing elements
    rb_tree.remove(10);
    assert(rb_tree.size() == 2);

    rb_tree.remove(5);
    rb_tree.remove(15);
    assert(rb_tree.empty());
    assert(rb_tree.size() == 0);

    std::cout << "✓ empty() and size() methods test passed" << std::endl;
}

// =============================================================================
// Red-Black Tree Property Tests
// =============================================================================

void test_red_black_properties_simple() {
    std::cout << "Testing Red-Black tree properties (simple case)..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;

    // Add elements that would create imbalance in regular BST
    for (int i = 1; i <= 7; ++i) {
        rb_tree.put(i, std::to_string(i));
    }

    // Tree should maintain logarithmic height
    int height = rb_tree.height();
    int expected_max_height = static_cast<int>(2 * std::log2(rb_tree.size() + 1));
    assert(height <= expected_max_height);

    // All operations should still work correctly
    for (int i = 1; i <= 7; ++i) {
        assert(rb_tree.contains(i));
        assert(rb_tree.get(i) == std::to_string(i));
    }

    // Keys should be in sorted order
    std::vector<int> keys = rb_tree.keys();
    assert(is_sorted(keys));

    std::cout << "✓ Red-Black tree properties (simple) test passed" << std::endl;
}

void test_red_black_properties_complex() {
    std::cout << "Testing Red-Black tree properties (complex case)..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;

    // Insert elements in worst-case order for regular BST (sorted)
    const int num_elements = 31;  // 2^5 - 1
    for (int i = 1; i <= num_elements; ++i) {
        rb_tree.put(i, std::to_string(i));
    }

    // Verify tree maintains reasonable height
    int height = rb_tree.height();
    int theoretical_min_height = static_cast<int>(std::log2(num_elements));
    int theoretical_max_height = static_cast<int>(2 * std::log2(num_elements + 1));

    assert(height >= theoretical_min_height);
    assert(height <= theoretical_max_height);

    std::cout << "  Height: " << height << " (min: " << theoretical_min_height
              << ", max: " << theoretical_max_height << ")" << std::endl;

    // Verify all elements are present and accessible
    for (int i = 1; i <= num_elements; ++i) {
        assert(rb_tree.contains(i));
        assert(rb_tree.get(i) == std::to_string(i));
    }

    std::cout << "✓ Red-Black tree properties (complex) test passed" << std::endl;
}

void test_balance_after_deletions() {
    std::cout << "Testing balance maintenance after deletions..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;

    // Build a tree
    std::vector<int> keys = {50, 25, 75, 10, 30, 60, 80, 5, 15, 27, 35};
    for (int key : keys) {
        rb_tree.put(key, std::to_string(key));
    }

    int initial_height = rb_tree.height();

    // Remove several elements
    rb_tree.remove(5);
    rb_tree.remove(15);
    rb_tree.remove(27);

    // Tree should still be reasonably balanced
    int final_height = rb_tree.height();
    assert(final_height <= initial_height + 1);  // Height shouldn't increase significantly

    // Verify remaining elements
    std::vector<int> remaining = {50, 25, 75, 10, 30, 60, 80, 35};
    for (int key : remaining) {
        assert(rb_tree.contains(key));
    }

    // Verify removed elements are gone
    assert(!rb_tree.contains(5));
    assert(!rb_tree.contains(15));
    assert(!rb_tree.contains(27));

    std::cout << "✓ Balance maintenance after deletions test passed" << std::endl;
}

// =============================================================================
// Put Operation Tests
// =============================================================================

void test_put_operations() {
    std::cout << "Testing put() operations..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;

    // Test inserting new keys
    rb_tree.put(10, "ten");
    assert(rb_tree.size() == 1);
    assert(rb_tree.get(10) == "ten");

    rb_tree.put(5, "five");
    rb_tree.put(15, "fifteen");
    assert(rb_tree.size() == 3);

    // Test updating existing keys
    rb_tree.put(10, "TEN");
    assert(rb_tree.size() == 3);  // Size shouldn't change
    assert(rb_tree.get(10) == "TEN");  // Value should be updated

    // Test maintaining order
    std::vector<int> keys = rb_tree.keys();
    std::vector<int> expected = {5, 10, 15};
    assert(vectors_equal(keys, expected));

    std::cout << "✓ put() operations test passed" << std::endl;
}

void test_put_sequential_ascending() {
    std::cout << "Testing put() with sequential ascending keys..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;

    // This would create a linear tree in regular BST
    const int num_keys = 20;
    for (int i = 1; i <= num_keys; ++i) {
        rb_tree.put(i, std::to_string(i));
    }

    // Verify tree remains balanced
    int height = rb_tree.height();
    int max_expected_height = static_cast<int>(2 * std::log2(num_keys + 1));
    assert(height <= max_expected_height);

    // Verify all keys are present
    for (int i = 1; i <= num_keys; ++i) {
        assert(rb_tree.contains(i));
    }

    std::cout << "✓ put() sequential ascending test passed" << std::endl;
}

void test_put_sequential_descending() {
    std::cout << "Testing put() with sequential descending keys..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;

    // Insert in descending order
    const int num_keys = 20;
    for (int i = num_keys; i >= 1; --i) {
        rb_tree.put(i, std::to_string(i));
    }

    // Verify tree remains balanced
    int height = rb_tree.height();
    int max_expected_height = static_cast<int>(2 * std::log2(num_keys + 1));
    assert(height <= max_expected_height);

    // Verify keys are in sorted order
    std::vector<int> keys = rb_tree.keys();
    assert(is_sorted(keys));
    assert(keys.size() == num_keys);

    std::cout << "✓ put() sequential descending test passed" << std::endl;
}

void test_put_random_order() {
    std::cout << "Testing put() with random order keys..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;

    // Generate random keys
    std::vector<int> random_keys = generate_random_integers(50, 1, 100);

    for (int key : random_keys) {
        rb_tree.put(key, std::to_string(key));
    }

    // Remove duplicates for verification
    std::set<int> unique_keys(random_keys.begin(), random_keys.end());
    assert(rb_tree.size() == static_cast<int>(unique_keys.size()));

    // Verify all unique keys are present
    for (int key : unique_keys) {
        assert(rb_tree.contains(key));
        assert(rb_tree.get(key) == std::to_string(key));
    }

    // Verify tree maintains reasonable height
    int height = rb_tree.height();
    int max_expected_height = static_cast<int>(2 * std::log2(rb_tree.size() + 1));
    assert(height <= max_expected_height);

    std::cout << "✓ put() random order test passed" << std::endl;
}

// =============================================================================
// Get and Contains Tests
// =============================================================================

void test_get_operations() {
    std::cout << "Testing get() operations..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;

    // Test get on empty tree
    assert(throws_specific_exception<std::invalid_argument>([&rb_tree]() { rb_tree.get(1); }));

    // Add test data
    rb_tree.put(10, "ten");
    rb_tree.put(5, "five");
    rb_tree.put(15, "fifteen");
    rb_tree.put(3, "three");
    rb_tree.put(7, "seven");

    // Test getting existing keys
    assert(rb_tree.get(10) == "ten");
    assert(rb_tree.get(5) == "five");
    assert(rb_tree.get(15) == "fifteen");
    assert(rb_tree.get(3) == "three");
    assert(rb_tree.get(7) == "seven");

    // Test getting non-existent keys
    assert(throws_specific_exception<std::invalid_argument>([&rb_tree]() { rb_tree.get(1); }));
    assert(throws_specific_exception<std::invalid_argument>([&rb_tree]() { rb_tree.get(20); }));

    std::cout << "✓ get() operations test passed" << std::endl;
}

void test_contains_operations() {
    std::cout << "Testing contains() operations..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;

    // Test contains on empty tree
    assert(!rb_tree.contains(1));

    // Add test data
    std::vector<int> keys = {50, 25, 75, 10, 30, 60, 80};
    for (int key : keys) {
        rb_tree.put(key, std::to_string(key));
    }

    // Test existing keys
    for (int key : keys) {
        assert(rb_tree.contains(key));
    }

    // Test non-existing keys
    std::vector<int> non_existing = {1, 15, 35, 55, 70, 90, 100};
    for (int key : non_existing) {
        assert(!rb_tree.contains(key));
    }

    std::cout << "✓ contains() operations test passed" << std::endl;
}

// =============================================================================
// Remove Operation Tests
// =============================================================================

void test_remove_operations() {
    std::cout << "Testing remove() operations..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;

    // Test remove on empty tree
    assert(throws_specific_exception<std::invalid_argument>([&rb_tree]() { rb_tree.remove(1); }));

    // Build test tree
    std::vector<int> keys = {50, 25, 75, 10, 30, 60, 80, 5, 15, 27, 35, 55, 65, 70, 90};
    for (int key : keys) {
        rb_tree.put(key, std::to_string(key));
    }

    int initial_size = rb_tree.size();

    // Remove leaf nodes
    rb_tree.remove(5);
    rb_tree.remove(70);
    assert(rb_tree.size() == initial_size - 2);
    assert(!rb_tree.contains(5));
    assert(!rb_tree.contains(70));

    // Remove nodes with one child
    rb_tree.remove(65);  // Assuming it has one child
    assert(!rb_tree.contains(65));

    // Remove nodes with two children
    rb_tree.remove(25);  // Should have two children
    assert(!rb_tree.contains(25));

    // Verify tree maintains order and balance
    std::vector<int> remaining_keys = rb_tree.keys();
    assert(is_sorted(remaining_keys));

    // Verify reasonable height after removals
    int height = rb_tree.height();
    int max_expected_height = static_cast<int>(2 * std::log2(rb_tree.size() + 1));
    assert(height <= max_expected_height);

    std::cout << "✓ remove() operations test passed" << std::endl;
}

void test_remove_root() {
    std::cout << "Testing remove() root node..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;

    // Single node tree
    rb_tree.put(10, "ten");
    rb_tree.remove(10);
    assert(rb_tree.empty());

    // Multi-node tree
    std::vector<int> keys = {50, 25, 75, 10, 30, 60, 80};
    for (int key : keys) {
        rb_tree.put(key, std::to_string(key));
    }

    rb_tree.remove(50);  // Remove root
    assert(rb_tree.size() == 6);
    assert(!rb_tree.contains(50));

    // Verify all other keys are still present
    for (int key : keys) {
        if (key != 50) {
            assert(rb_tree.contains(key));
        }
    }

    // Verify tree maintains properties
    std::vector<int> remaining_keys = rb_tree.keys();
    assert(is_sorted(remaining_keys));

    std::cout << "✓ remove() root node test passed" << std::endl;
}

void test_remove_all_elements() {
    std::cout << "Testing remove() all elements..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;

    // Add elements
    std::vector<int> keys = {15, 10, 20, 8, 12, 17, 25, 6, 11, 13, 16, 27};
    for (int key : keys) {
        rb_tree.put(key, std::to_string(key));
    }

    // Remove all elements in random order
    std::random_shuffle(keys.begin(), keys.end());

    for (int key : keys) {
        int size_before = rb_tree.size();
        rb_tree.remove(key);
        assert(rb_tree.size() == size_before - 1);
        assert(!rb_tree.contains(key));

        // Verify remaining elements maintain order
        if (!rb_tree.empty()) {
            std::vector<int> remaining = rb_tree.keys();
            assert(is_sorted(remaining));
        }
    }

    assert(rb_tree.empty());

    std::cout << "✓ remove() all elements test passed" << std::endl;
}

// =============================================================================
// Min/Max and Remove Min/Max Tests
// =============================================================================

void test_min_max_operations() {
    std::cout << "Testing min() and max() operations..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;

    // Empty tree should throw
    assert(throws_specific_exception<std::underflow_error>([&rb_tree]() { rb_tree.min(); }));
    assert(throws_specific_exception<std::underflow_error>([&rb_tree]() { rb_tree.max(); }));

    // Single element
    rb_tree.put(10, "ten");
    assert(rb_tree.min() == 10);
    assert(rb_tree.max() == 10);

    // Multiple elements
    std::vector<int> keys = {5, 15, 3, 12, 18, 1, 7, 20};
    for (int key : keys) {
        rb_tree.put(key, std::to_string(key));
    }

    assert(rb_tree.min() == 1);
    assert(rb_tree.max() == 20);

    std::cout << "✓ min() and max() operations test passed" << std::endl;
}

void test_remove_min_max() {
    std::cout << "Testing removeMin() and removeMax()..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;

    // Empty tree should throw
    assert(throws_specific_exception<std::underflow_error>([&rb_tree]() { rb_tree.removeMin(); }));
    assert(throws_specific_exception<std::underflow_error>([&rb_tree]() { rb_tree.removeMax(); }));

    // Add test data
    std::vector<int> keys = {50, 25, 75, 10, 30, 60, 80, 5, 15, 35, 90};
    for (int key : keys) {
        rb_tree.put(key, std::to_string(key));
    }

    // Remove minimum several times
    assert(rb_tree.min() == 5);
    rb_tree.removeMin();
    assert(rb_tree.min() == 10);
    assert(!rb_tree.contains(5));

    rb_tree.removeMin();
    assert(rb_tree.min() == 15);
    assert(!rb_tree.contains(10));

    // Remove maximum several times
    assert(rb_tree.max() == 90);
    rb_tree.removeMax();
    assert(rb_tree.max() == 80);
    assert(!rb_tree.contains(90));

    rb_tree.removeMax();
    assert(rb_tree.max() == 75);
    assert(!rb_tree.contains(80));

    // Verify tree maintains properties
    std::vector<int> remaining_keys = rb_tree.keys();
    assert(is_sorted(remaining_keys));

    std::cout << "✓ removeMin() and removeMax() test passed" << std::endl;
}

// =============================================================================
// Select and Rank Tests
// =============================================================================

void test_select_operations() {
    std::cout << "Testing select() operations..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;

    // Test bounds on empty tree
    assert(throws_specific_exception<std::invalid_argument>([&rb_tree]() { rb_tree.select(0); }));

    // Add elements: 10, 20, 30, 40, 50
    for (int i = 1; i <= 5; ++i) {
        rb_tree.put(i * 10, std::to_string(i * 10));
    }

    // Test select operations
    assert(rb_tree.select(0) == 10);
    assert(rb_tree.select(1) == 20);
    assert(rb_tree.select(2) == 30);
    assert(rb_tree.select(3) == 40);
    assert(rb_tree.select(4) == 50);

    // Test bounds
    assert(throws_specific_exception<std::invalid_argument>([&rb_tree]() { rb_tree.select(-1); }));
    assert(throws_specific_exception<std::invalid_argument>([&rb_tree]() { rb_tree.select(5); }));

    std::cout << "✓ select() operations test passed" << std::endl;
}

void test_rank_operations() {
    std::cout << "Testing rank() operations..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;

    // Add elements: 10, 30, 50, 70, 90
    std::vector<int> keys = {50, 30, 70, 10, 90};
    for (int key : keys) {
        rb_tree.put(key, std::to_string(key));
    }

    // Test rank of existing keys
    assert(rb_tree.rank(10) == 0);
    assert(rb_tree.rank(30) == 1);
    assert(rb_tree.rank(50) == 2);
    assert(rb_tree.rank(70) == 3);
    assert(rb_tree.rank(90) == 4);

    // Test rank of non-existing keys
    assert(rb_tree.rank(5) == 0);   // Smaller than all
    assert(rb_tree.rank(25) == 1);  // Between 10 and 30
    assert(rb_tree.rank(60) == 3);  // Between 50 and 70
    assert(rb_tree.rank(100) == 5); // Larger than all

    std::cout << "✓ rank() operations test passed" << std::endl;
}

void test_select_rank_consistency() {
    std::cout << "Testing select() and rank() consistency..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;

    // Add random elements
    std::vector<int> keys = generate_random_integers(20, 1, 100);
    for (int key : keys) {
        rb_tree.put(key, std::to_string(key));
    }

    // Test select(rank(key)) == key for all keys
    std::vector<int> all_keys = rb_tree.keys();
    for (int key : all_keys) {
        assert(rb_tree.select(rb_tree.rank(key)) == key);
    }

    // Test rank(select(i)) == i for all valid indices
    for (int i = 0; i < rb_tree.size(); ++i) {
        assert(rb_tree.rank(rb_tree.select(i)) == i);
    }

    std::cout << "✓ select() and rank() consistency test passed" << std::endl;
}

// =============================================================================
// Floor and Ceiling Tests
// =============================================================================

void test_floor_operations() {
    std::cout << "Testing floor() operations..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;

    // Empty tree should throw
    assert(throws_specific_exception<std::underflow_error>([&rb_tree]() { rb_tree.floor(10); }));

    // Add elements: 10, 30, 50, 70, 90
    std::vector<int> keys = {50, 30, 70, 10, 90};
    for (int key : keys) {
        rb_tree.put(key, std::to_string(key));
    }

    // Test floor operations
    assert(rb_tree.floor(10) == 10);  // Exact match
    assert(rb_tree.floor(25) == 10);  // Floor of 25 is 10
    assert(rb_tree.floor(30) == 30);  // Exact match
    assert(rb_tree.floor(65) == 50);  // Floor of 65 is 50
    assert(rb_tree.floor(90) == 90);  // Exact match
    assert(rb_tree.floor(100) == 90); // Floor of 100 is 90

    // Key too small should throw
    assert(throws_specific_exception<std::invalid_argument>([&rb_tree]() { rb_tree.floor(5); }));

    std::cout << "✓ floor() operations test passed" << std::endl;
}

void test_ceiling_operations() {
    std::cout << "Testing ceiling() operations..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;

    // Empty tree should throw
    assert(throws_specific_exception<std::underflow_error>([&rb_tree]() { rb_tree.ceiling(10); }));

    // Add elements: 10, 30, 50, 70, 90
    std::vector<int> keys = {50, 30, 70, 10, 90};
    for (int key : keys) {
        rb_tree.put(key, std::to_string(key));
    }

    // Test ceiling operations
    assert(rb_tree.ceiling(5) == 10);   // Ceiling of 5 is 10
    assert(rb_tree.ceiling(10) == 10);  // Exact match
    assert(rb_tree.ceiling(25) == 30);  // Ceiling of 25 is 30
    assert(rb_tree.ceiling(30) == 30);  // Exact match
    assert(rb_tree.ceiling(65) == 70);  // Ceiling of 65 is 70
    assert(rb_tree.ceiling(90) == 90);  // Exact match

    // Key too large should throw
    assert(throws_specific_exception<std::invalid_argument>([&rb_tree]() { rb_tree.ceiling(100); }));

    std::cout << "✓ ceiling() operations test passed" << std::endl;
}

// =============================================================================
// Range Operations Tests
// =============================================================================

void test_range_size() {
    std::cout << "Testing range size() operations..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;

    // Empty tree
    assert(rb_tree.size(10, 20) == 0);

    // Add elements: 10, 20, 30, 40, 50, 60, 70, 80, 90
    for (int i = 1; i <= 9; ++i) {
        rb_tree.put(i * 10, std::to_string(i * 10));
    }

    // Test various ranges
    assert(rb_tree.size(10, 90) == 9);  // All elements
    assert(rb_tree.size(20, 60) == 5);  // 20, 30, 40, 50, 60
    assert(rb_tree.size(25, 55) == 3);  // 30, 40, 50
    assert(rb_tree.size(15, 25) == 1);  // 20
    assert(rb_tree.size(95, 100) == 0); // No elements
    assert(rb_tree.size(50, 30) == 0);  // Invalid range

    std::cout << "✓ range size() operations test passed" << std::endl;
}

void test_keys_operations() {
    std::cout << "Testing keys() operations..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;

    // Empty tree
    std::vector<int> empty_keys = rb_tree.keys();
    assert(empty_keys.empty());

    // Add elements in random order
    std::vector<int> input_keys = {50, 25, 75, 10, 60, 80, 30, 5, 15};
    for (int key : input_keys) {
        rb_tree.put(key, std::to_string(key));
    }

    // Test all keys (should be sorted)
    std::vector<int> all_keys = rb_tree.keys();
    assert(all_keys.size() == input_keys.size());
    assert(is_sorted(all_keys));

    // Test range keys
    std::vector<int> range_keys = rb_tree.keys(20, 70);
    std::vector<int> expected_range = {25, 30, 50, 60};
    assert(vectors_equal(range_keys, expected_range));

    // Test empty range
    std::vector<int> empty_range = rb_tree.keys(90, 100);
    assert(empty_range.empty());

    std::cout << "✓ keys() operations test passed" << std::endl;
}

// =============================================================================
// Tree Structure Tests
// =============================================================================

void test_height_operations() {
    std::cout << "Testing height() operations..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;

    // Empty tree
    assert(rb_tree.height() == -1);

    // Single node
    rb_tree.put(10, "ten");
    assert(rb_tree.height() == 0);

    // Build larger tree and verify reasonable height
    for (int i = 1; i <= 15; ++i) {
        rb_tree.put(i, std::to_string(i));
    }

    int height = rb_tree.height();
    int theoretical_min = static_cast<int>(std::log2(15));
    int theoretical_max = static_cast<int>(2 * std::log2(15 + 1));

    assert(height >= theoretical_min);
    assert(height <= theoretical_max);

    std::cout << "  Tree with 15 elements has height: " << height << std::endl;
    std::cout << "✓ height() operations test passed" << std::endl;
}


// =============================================================================
// Performance and Stress Tests
// =============================================================================

void test_large_dataset_performance() {
    std::cout << "Testing large dataset performance..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;
    const int num_elements = 10000;

    auto start_time = std::chrono::high_resolution_clock::now();

    // Insert elements
    for (int i = 0; i < num_elements; ++i) {
        rb_tree.put(i, std::to_string(i));
    }

    auto insert_time = std::chrono::high_resolution_clock::now();

    // Verify size and height
    assert(rb_tree.size() == num_elements);
    int height = rb_tree.height();
    int max_expected_height = static_cast<int>(2 * std::log2(num_elements + 1));
    assert(height <= max_expected_height);

    // Perform searches
    for (int i = 0; i < 1000; ++i) {
        int key = i * (num_elements / 1000);
        assert(rb_tree.contains(key));
        assert(rb_tree.get(key) == std::to_string(key));
    }

    auto search_time = std::chrono::high_resolution_clock::now();

    // Perform deletions
    for (int i = 0; i < 1000; ++i) {
        int key = i * 2;  // Delete every other element in first 2000
        if (rb_tree.contains(key)) {
            rb_tree.remove(key);
        }
    }

    auto delete_time = std::chrono::high_resolution_clock::now();

    // Calculate and display times
    auto insert_duration = std::chrono::duration_cast<std::chrono::milliseconds>(insert_time - start_time);
    auto search_duration = std::chrono::duration_cast<std::chrono::milliseconds>(search_time - insert_time);
    auto delete_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delete_time - search_time);

    std::cout << "  Insert " << num_elements << " elements: " << insert_duration.count() << "ms" << std::endl;
    std::cout << "  Search 1000 elements: " << search_duration.count() << "ms" << std::endl;
    std::cout << "  Delete 1000 elements: " << delete_duration.count() << "ms" << std::endl;
    std::cout << "  Final height: " << rb_tree.height() << " (max expected: " << max_expected_height << ")" << std::endl;

    std::cout << "✓ Large dataset performance test passed" << std::endl;
}

void test_worst_case_insertions() {
    std::cout << "Testing worst-case insertion patterns..." << std::endl;

    // Test 1: Sequential ascending
    {
        RedBlackBST<int, std::string> rb_tree;
        const int num_elements = 1000;

        for (int i = 1; i <= num_elements; ++i) {
            rb_tree.put(i, std::to_string(i));
        }

        int height = rb_tree.height();
        int max_expected = static_cast<int>(2 * std::log2(num_elements + 1));
        assert(height <= max_expected);

        std::cout << "  Sequential ascending (" << num_elements << " elements): height = " << height << std::endl;
    }

    // Test 2: Sequential descending
    {
        RedBlackBST<int, std::string> rb_tree;
        const int num_elements = 1000;

        for (int i = num_elements; i >= 1; --i) {
            rb_tree.put(i, std::to_string(i));
        }

        int height = rb_tree.height();
        int max_expected = static_cast<int>(2 * std::log2(num_elements + 1));
        assert(height <= max_expected);

        std::cout << "  Sequential descending (" << num_elements << " elements): height = " << height << std::endl;
    }

    // Test 3: Alternating pattern
    {
        RedBlackBST<int, std::string> rb_tree;
        const int num_elements = 1000;

        // Insert in alternating pattern: 1, 1000, 2, 999, 3, 998, ...
        for (int i = 1; i <= num_elements / 2; ++i) {
            rb_tree.put(i, std::to_string(i));
            rb_tree.put(num_elements - i + 1, std::to_string(num_elements - i + 1));
        }

        int height = rb_tree.height();
        int max_expected = static_cast<int>(2 * std::log2(num_elements + 1));
        assert(height <= max_expected);

        std::cout << "  Alternating pattern (" << num_elements << " elements): height = " << height << std::endl;
    }

    std::cout << "✓ Worst-case insertion patterns test passed" << std::endl;
}

// =============================================================================
// Edge Cases and Error Handling Tests
// =============================================================================

void test_edge_cases() {
    std::cout << "Testing edge cases..." << std::endl;

    // Single element operations
    {
        RedBlackBST<int, std::string> rb_tree;
        rb_tree.put(42, "answer");

        assert(rb_tree.size() == 1);
        assert(!rb_tree.empty());
        assert(rb_tree.min() == 42);
        assert(rb_tree.max() == 42);
        assert(rb_tree.select(0) == 42);
        assert(rb_tree.rank(42) == 0);
        assert(rb_tree.floor(42) == 42);
        assert(rb_tree.ceiling(42) == 42);
        assert(rb_tree.height() == 0);

        rb_tree.remove(42);
        assert(rb_tree.empty());
    }

    // Duplicate key handling
    {
        RedBlackBST<int, std::string> rb_tree;
        rb_tree.put(10, "first");
        rb_tree.put(10, "second");
        rb_tree.put(10, "third");

        assert(rb_tree.size() == 1);
        assert(rb_tree.get(10) == "third");
    }

    std::cout << "✓ Edge cases test passed" << std::endl;
}

void test_string_keys() {
    std::cout << "Testing with string keys..." << std::endl;

    RedBlackBST<std::string, int> rb_tree;

    std::vector<std::string> names = {"charlie", "alice", "bob", "david", "eve", "frank"};
    int value = 1;
    for (const std::string& name : names) {
        rb_tree.put(name, value++);
    }

    // Should be sorted alphabetically
    std::vector<std::string> keys = rb_tree.keys();
    assert(is_sorted(keys));

    assert(rb_tree.min() == "alice");
    assert(rb_tree.max() == "frank");

    // Test operations
    assert(rb_tree.contains("charlie"));
    assert(rb_tree.get("bob") > 0);

    rb_tree.remove("charlie");
    assert(!rb_tree.contains("charlie"));
    assert(rb_tree.size() == names.size() - 1);

    std::cout << "✓ String keys test passed" << std::endl;
}

// =============================================================================
// Integration Tests
// =============================================================================

void test_complex_workflow() {
    std::cout << "Testing complex workflow..." << std::endl;

    RedBlackBST<int, std::string> rb_tree;

    // Phase 1: Build initial tree
    std::vector<int> initial_keys = {50, 25, 75, 10, 30, 60, 80, 5, 15, 27, 35, 55, 65, 70, 90};
    for (int key : initial_keys) {
        rb_tree.put(key, "initial_" + std::to_string(key));
    }

    assert(rb_tree.size() == initial_keys.size());
    int initial_height = rb_tree.height();

    // Phase 2: Update some values
    std::vector<int> update_keys = {25, 60, 90};
    for (int key : update_keys) {
        rb_tree.put(key, "updated_" + std::to_string(key));
    }

    assert(rb_tree.size() == initial_keys.size());  // Size shouldn't change
    for (int key : update_keys) {
        assert(rb_tree.get(key) == "updated_" + std::to_string(key));
    }

    // Phase 3: Add more elements
    std::vector<int> additional_keys = {3, 7, 12, 22, 33, 45, 67, 85, 95};
    for (int key : additional_keys) {
        rb_tree.put(key, "additional_" + std::to_string(key));
    }

    // Phase 4: Remove some elements
    std::vector<int> remove_keys = {5, 27, 65, 90};
    for (int key : remove_keys) {
        rb_tree.remove(key);
    }

    // Phase 5: Verify final state
    int expected_size = initial_keys.size() + additional_keys.size() - remove_keys.size();
    assert(rb_tree.size() == expected_size);

    // Verify removed keys are gone
    for (int key : remove_keys) {
        assert(!rb_tree.contains(key));
    }

    // Verify tree maintains reasonable height
    int final_height = rb_tree.height();
    int max_expected_height = static_cast<int>(2 * std::log2(rb_tree.size() + 1));
    assert(final_height <= max_expected_height);

    // Verify all keys are in sorted order
    std::vector<int> final_keys = rb_tree.keys();
    assert(is_sorted(final_keys));
    assert(final_keys.size() == rb_tree.size());

    std::cout << "  Initial height: " << initial_height << ", Final height: " << final_height << std::endl;
    std::cout << "✓ Complex workflow test passed" << std::endl;
}

// =============================================================================
// Main Test Runner
// =============================================================================

void run_all_tests() {
    std::cout << "=== Running Red-Black BST Tests ===" << std::endl;
    std::cout << std::endl;

    // Constructor and Basic State Tests
    test_default_constructor();
    test_destructor();
    test_empty_and_size();
    std::cout << std::endl;

    // Red-Black Tree Property Tests
    test_red_black_properties_simple();
    test_red_black_properties_complex();
    test_balance_after_deletions();
    std::cout << std::endl;

    // Put Operation Tests
    test_put_operations();
    test_put_sequential_ascending();
    test_put_sequential_descending();
    test_put_random_order();
    std::cout << std::endl;

    // Get and Contains Tests
    test_get_operations();
    test_contains_operations();
    std::cout << std::endl;

    // Remove Operation Tests
    test_remove_operations();
    test_remove_root();
    test_remove_all_elements();
    std::cout << std::endl;

    // Min/Max and Remove Min/Max Tests
    test_min_max_operations();
    test_remove_min_max();
    std::cout << std::endl;

    // Select and Rank Tests
    test_select_operations();
    test_rank_operations();
    test_select_rank_consistency();
    std::cout << std::endl;

    // Floor and Ceiling Tests
    test_floor_operations();
    test_ceiling_operations();
    std::cout << std::endl;

    // Range Operations Tests
    test_range_size();
    test_keys_operations();
    std::cout << std::endl;

    // Tree Structure Tests
    test_height_operations();
    std::cout << std::endl;

    // Performance and Stress Tests
    test_large_dataset_performance();
    test_worst_case_insertions();
    std::cout << std::endl;

    // Edge Cases and Error Handling Tests
    test_edge_cases();
    test_string_keys();
    std::cout << std::endl;

    // Integration Tests
    test_complex_workflow();
    std::cout << std::endl;

    std::cout << "=== All Red-Black BST Tests Passed Successfully! ===" << std::endl;
}

int main() {
    try {
        run_all_tests();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Test failed with unknown exception" << std::endl;
        return 1;
    }
}