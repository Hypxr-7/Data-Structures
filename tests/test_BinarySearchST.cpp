#pragma once

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../data_structures/BinarySearchST.hpp"

/**
 * @brief Test suite for BinarySearchST using simple assert statements
 *
 * This test suite provides comprehensive testing of the Binary Search Symbol Table
 * using only standard library assert statements. Tests are organized by functionality
 * and include edge cases, error conditions, ordered operations, and integration scenarios.
 */

// Helper function to test if exception is thrown
template<typename Func>
bool throws_exception(Func func) {
    try {
        func();
        return false;  // No exception thrown
    } catch (...) {
        return true;   // Exception was thrown
    }
}

// Helper function to test specific exception type
template<typename ExceptionType, typename Func>
bool throws_specific_exception(Func func) {
    try {
        func();
        return false;  // No exception thrown
    } catch (const ExceptionType&) {
        return true;   // Correct exception type thrown
    } catch (...) {
        return false;  // Wrong exception type
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

// =============================================================================
// Constructor and Basic State Tests
// =============================================================================

void test_default_constructor() {
    std::cout << "Testing default constructor..." << std::endl;

    BinarySearchST<int, std::string> st_int;
    BinarySearchST<std::string, int> st_string;

    // Should create empty symbol tables
    assert(st_int.empty());
    assert(st_int.size() == 0);
    assert(st_string.empty());
    assert(st_string.size() == 0);

    std::cout << "✓ Default constructor test passed" << std::endl;
}

void test_empty_method() {
    std::cout << "Testing empty() method..." << std::endl;

    BinarySearchST<int, std::string> st;

    // Initially empty
    assert(st.empty());

    // After adding element
    st.put(1, "test");
    assert(!st.empty());

    // After removing element
    st.remove(1);
    assert(st.empty());

    std::cout << "✓ empty() method test passed" << std::endl;
}

void test_size_method() {
    std::cout << "Testing size() method..." << std::endl;

    BinarySearchST<int, std::string> st;

    assert(st.size() == 0);

    // Add elements
    st.put(3, "three");
    assert(st.size() == 1);

    st.put(1, "one");
    assert(st.size() == 2);

    st.put(2, "two");
    assert(st.size() == 3);

    // Remove element
    st.remove(2);
    assert(st.size() == 2);

    // Remove all elements
    st.remove(1);
    st.remove(3);
    assert(st.size() == 0);

    std::cout << "✓ size() method test passed" << std::endl;
}

// =============================================================================
// Put Operation Tests
// =============================================================================

void test_put_new_keys() {
    std::cout << "Testing put() with new keys..." << std::endl;

    BinarySearchST<int, std::string> st;

    // Test adding new keys (should maintain sorted order)
    st.put(3, "three");
    assert(st.size() == 1);
    assert(!st.empty());

    st.put(1, "one");
    assert(st.size() == 2);

    st.put(2, "two");
    assert(st.size() == 3);

    // Verify sorted order
    assert(st.select(0) == 1);
    assert(st.select(1) == 2);
    assert(st.select(2) == 3);

    std::cout << "✓ put() new keys test passed" << std::endl;
}

void test_put_update_existing() {
    std::cout << "Testing put() updates existing keys..." << std::endl;

    BinarySearchST<int, std::string> st;

    // Add initial key-value pair
    st.put(1, "one");
    assert(st.size() == 1);

    // Update same key with new value
    st.put(1, "ONE");
    assert(st.size() == 1);  // Size should not change
    assert(st.get(1) == "ONE");  // Value should be updated

    // Update again
    st.put(1, "updated");
    assert(st.size() == 1);
    assert(st.get(1) == "updated");

    std::cout << "✓ put() update existing test passed" << std::endl;
}

void test_put_maintains_order() {
    std::cout << "Testing put() maintains sorted order..." << std::endl;

    BinarySearchST<int, std::string> st;

    // Add keys in random order
    std::vector<int> keys = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    for (int key : keys) {
        st.put(key, std::to_string(key));
    }

    assert(st.size() == 9);

    // Verify sorted order
    for (int i = 0; i < 9; ++i) {
        assert(st.select(i) == i + 1);
    }

    std::cout << "✓ put() maintains order test passed" << std::endl;
}

void test_put_large_dataset() {
    std::cout << "Testing put() with large dataset (resizing)..." << std::endl;

    BinarySearchST<int, std::string> st;

    // Add many elements to test resizing
    const int num_elements = 100;
    for (int i = 0; i < num_elements; ++i) {
        st.put(i, std::to_string(i));
    }

    assert(st.size() == num_elements);

    // Verify all elements are present and in order
    for (int i = 0; i < num_elements; ++i) {
        assert(st.contains(i));
        assert(st.get(i) == std::to_string(i));
        assert(st.select(i) == i);
    }

    std::cout << "✓ put() large dataset test passed" << std::endl;
}

// =============================================================================
// Get Operation Tests
// =============================================================================

void test_get_existing_keys() {
    std::cout << "Testing get() with existing keys..." << std::endl;

    BinarySearchST<int, std::string> st;

    // Add test data
    st.put(2, "two");
    st.put(1, "one");
    st.put(3, "three");

    // Test getting existing keys
    assert(st.get(1) == "one");
    assert(st.get(2) == "two");
    assert(st.get(3) == "three");

    std::cout << "✓ get() existing keys test passed" << std::endl;
}

void test_get_empty_table() {
    std::cout << "Testing get() on empty table..." << std::endl;

    BinarySearchST<int, std::string> st;

    // Should throw underflow_error
    assert(throws_specific_exception<std::underflow_error>([&st]() { st.get(1); }));

    std::cout << "✓ get() empty table test passed" << std::endl;
}

void test_get_nonexistent_keys() {
    std::cout << "Testing get() with non-existent keys..." << std::endl;

    BinarySearchST<int, std::string> st;

    // Populated table but key doesn't exist
    st.put(1, "one");
    st.put(3, "three");

    // Should throw invalid_argument for non-existent key
    assert(throws_specific_exception<std::invalid_argument>([&st]() { st.get(2); }));
    assert(throws_specific_exception<std::invalid_argument>([&st]() { st.get(10); }));

    std::cout << "✓ get() non-existent keys test passed" << std::endl;
}

// =============================================================================
// Contains Operation Tests
// =============================================================================

void test_contains_existing_keys() {
    std::cout << "Testing contains() with existing keys..." << std::endl;

    BinarySearchST<int, std::string> st;

    // Add test data
    st.put(2, "two");
    st.put(1, "one");
    st.put(3, "three");

    assert(st.contains(1));
    assert(st.contains(2));
    assert(st.contains(3));

    std::cout << "✓ contains() existing keys test passed" << std::endl;
}

void test_contains_empty_table() {
    std::cout << "Testing contains() on empty table..." << std::endl;

    BinarySearchST<int, std::string> st;

    std::cout << "✓ contains() empty table test passed" << std::endl;
}

void test_contains_nonexistent_keys() {
    std::cout << "Testing contains() with non-existent keys..." << std::endl;

    BinarySearchST<int, std::string> st;

    // Populated table but key doesn't exist
    st.put(1, "one");
    st.put(3, "three");

    assert(!st.contains(2));
    assert(!st.contains(0));
    assert(!st.contains(10));

    std::cout << "✓ contains() non-existent keys test passed" << std::endl;
}

// =============================================================================
// Remove Operation Tests
// =============================================================================

void test_remove_existing_keys() {
    std::cout << "Testing remove() with existing keys..." << std::endl;

    BinarySearchST<int, std::string> st;

    // Add test data
    st.put(1, "one");
    st.put(2, "two");
    st.put(3, "three");
    st.put(4, "four");
    st.put(5, "five");

    int initial_size = st.size();

    // Remove from middle
    st.remove(3);
    assert(st.size() == initial_size - 1);
    assert(!st.contains(3));

    // Verify order is maintained
    assert(st.select(0) == 1);
    assert(st.select(1) == 2);
    assert(st.select(2) == 4);
    assert(st.select(3) == 5);

    std::cout << "✓ remove() existing keys test passed" << std::endl;
}

void test_remove_empty_table() {
    std::cout << "Testing remove() on empty table..." << std::endl;

    BinarySearchST<int, std::string> st;

    // Should throw underflow_error
    assert(throws_specific_exception<std::underflow_error>([&st]() { st.remove(1); }));

    std::cout << "✓ remove() empty table test passed" << std::endl;
}

void test_remove_nonexistent_keys() {
    std::cout << "Testing remove() with non-existent keys..." << std::endl;

    BinarySearchST<int, std::string> st;

    // Populated table but key doesn't exist
    st.put(1, "one");
    st.put(3, "three");

    int initial_size = st.size();

    // Should not throw, just do nothing
    st.remove(2);
    st.remove(10);

    assert(st.size() == initial_size);  // Size unchanged

    std::cout << "✓ remove() non-existent keys test passed" << std::endl;
}

void test_remove_all_elements() {
    std::cout << "Testing remove() all elements..." << std::endl;

    BinarySearchST<int, std::string> st;

    // Add test data
    std::vector<int> keys = {3, 1, 4, 2, 5};
    for (int key : keys) {
        st.put(key, std::to_string(key));
    }

    // Remove all elements
    for (int key : keys) {
        int size_before = st.size();
        st.remove(key);
        assert(st.size() == size_before - 1);
        // Only check contains if table is not empty
        if (!st.empty()) {
            assert(!st.contains(key));
        }
    }

    assert(st.empty());
    assert(st.size() == 0);

    std::cout << "✓ remove() all elements test passed" << std::endl;
}

// =============================================================================
// Min/Max Operation Tests
// =============================================================================

void test_min_max_operations() {
    std::cout << "Testing min() and max() operations..." << std::endl;

    BinarySearchST<int, std::string> st;

    // Empty table should throw
    assert(throws_specific_exception<std::underflow_error>([&st]() { st.min(); }));
    assert(throws_specific_exception<std::underflow_error>([&st]() { st.max(); }));

    // Add elements in random order
    st.put(5, "five");
    st.put(2, "two");
    st.put(8, "eight");
    st.put(1, "one");
    st.put(9, "nine");

    assert(st.min() == 1);
    assert(st.max() == 9);

    // Remove min and max
    st.remove(1);
    st.remove(9);

    assert(st.min() == 2);
    assert(st.max() == 8);

    std::cout << "✓ min() and max() operations test passed" << std::endl;
}

void test_remove_min_max() {
    std::cout << "Testing removeMin() and removeMax()..." << std::endl;

    BinarySearchST<int, std::string> st;

    // Empty table should throw
    assert(throws_specific_exception<std::underflow_error>([&st]() { st.removeMin(); }));
    assert(throws_specific_exception<std::underflow_error>([&st]() { st.removeMax(); }));

    // Add elements
    st.put(3, "three");
    st.put(1, "one");
    st.put(5, "five");
    st.put(2, "two");
    st.put(4, "four");

    // Remove min
    st.removeMin();
    assert(st.size() == 4);
    assert(st.min() == 2);
    assert(!st.contains(1));

    // Remove max
    st.removeMax();
    assert(st.size() == 3);
    assert(st.max() == 4);
    assert(!st.contains(5));

    std::cout << "✓ removeMin() and removeMax() test passed" << std::endl;
}

// =============================================================================
// Select and Rank Operation Tests
// =============================================================================

void test_select_operations() {
    std::cout << "Testing select() operations..." << std::endl;

    BinarySearchST<int, std::string> st;

    // Add elements
    std::vector<int> keys = {3, 1, 4, 1, 5, 9, 2, 6};  // Note: duplicate 1 should be ignored
    for (int key : keys) {
        st.put(key, std::to_string(key));
    }

    // Test select operations
    assert(st.select(0) == 1);  // 0th element (smallest)
    assert(st.select(1) == 2);  // 1st element
    assert(st.select(2) == 3);  // 2nd element

    // Test bounds
    assert(throws_specific_exception<std::invalid_argument>([&st]() { st.select(-1); }));
    assert(throws_specific_exception<std::invalid_argument>([&st]() { st.select(st.size()); }));

    std::cout << "✓ select() operations test passed" << std::endl;
}

void test_rank_operations() {
    std::cout << "Testing rank() operations..." << std::endl;

    BinarySearchST<int, std::string> st;

    // Add elements: 1, 3, 5, 7, 9
    for (int i = 1; i <= 9; i += 2) {
        st.put(i, std::to_string(i));
    }

    // Test rank of existing keys
    assert(st.rank(1) == 0);
    assert(st.rank(3) == 1);
    assert(st.rank(5) == 2);
    assert(st.rank(7) == 3);
    assert(st.rank(9) == 4);

    // Test rank of non-existing keys (insertion points)
    assert(st.rank(0) == 0);  // Would be inserted at beginning
    assert(st.rank(2) == 1);  // Would be inserted between 1 and 3
    assert(st.rank(4) == 2);  // Would be inserted between 3 and 5
    assert(st.rank(10) == 5); // Would be inserted at end

    std::cout << "✓ rank() operations test passed" << std::endl;
}

void test_select_rank_consistency() {
    std::cout << "Testing select() and rank() consistency..." << std::endl;

    BinarySearchST<int, std::string> st;

    // Add test data
    for (int i = 0; i < 10; ++i) {
        st.put(i * 2, std::to_string(i * 2));  // Even numbers 0, 2, 4, ..., 18
    }

    // Test select(rank(key)) == key for existing keys
    for (int i = 0; i < st.size(); ++i) {
        int key = st.select(i);
        assert(st.rank(key) == i);
        assert(st.select(st.rank(key)) == key);
    }

    std::cout << "✓ select() and rank() consistency test passed" << std::endl;
}

// =============================================================================
// Floor and Ceiling Operation Tests
// =============================================================================

void test_floor_operations() {
    std::cout << "Testing floor() operations..." << std::endl;

    BinarySearchST<int, std::string> st;

    // Empty table should throw
    assert(throws_specific_exception<std::underflow_error>([&st]() { st.floor(5); }));

    // Add elements: 2, 4, 6, 8, 10
    for (int i = 2; i <= 10; i += 2) {
        st.put(i, std::to_string(i));
    }

    // Test floor operations
    assert(st.floor(2) == 2);   // Exact match
    assert(st.floor(3) == 2);   // Floor of 3 is 2
    assert(st.floor(5) == 4);   // Floor of 5 is 4
    assert(st.floor(10) == 10); // Exact match
    assert(st.floor(15) == 10); // Floor of 15 is 10

    // Key too small should throw
    assert(throws_specific_exception<std::invalid_argument>([&st]() { st.floor(1); }));

    std::cout << "✓ floor() operations test passed" << std::endl;
}

void test_ceiling_operations() {
    std::cout << "Testing ceiling() operations..." << std::endl;

    BinarySearchST<int, std::string> st;

    // Empty table should throw
    assert(throws_specific_exception<std::underflow_error>([&st]() { st.ceiling(5); }));

    // Add elements: 2, 4, 6, 8, 10
    for (int i = 2; i <= 10; i += 2) {
        st.put(i, std::to_string(i));
    }

    // Test ceiling operations
    assert(st.ceiling(2) == 2);   // Exact match
    assert(st.ceiling(1) == 2);   // Ceiling of 1 is 2
    assert(st.ceiling(3) == 4);   // Ceiling of 3 is 4
    assert(st.ceiling(5) == 6);   // Ceiling of 5 is 6
    assert(st.ceiling(10) == 10); // Exact match

    // Key too large should throw
    assert(throws_specific_exception<std::invalid_argument>([&st]() { st.ceiling(15); }));

    std::cout << "✓ ceiling() operations test passed" << std::endl;
}

// =============================================================================
// Range Operation Tests
// =============================================================================

void test_range_size() {
    std::cout << "Testing range size() operations..." << std::endl;

    BinarySearchST<int, std::string> st;

    // Empty table
    assert(st.size(1, 5) == 0);

    // Add elements: 1, 3, 5, 7, 9
    for (int i = 1; i <= 9; i += 2) {
        st.put(i, std::to_string(i));
    }

    // Test range sizes
    assert(st.size(1, 9) == 5);   // All elements
    assert(st.size(1, 5) == 3);   // 1, 3, 5
    assert(st.size(3, 7) == 3);   // 3, 5, 7
    assert(st.size(2, 8) == 3);   // 3, 5, 7
    assert(st.size(0, 10) == 5);  // All elements
    assert(st.size(10, 20) == 0); // No elements
    assert(st.size(5, 3) == 0);   // Invalid range (lo > hi)

    std::cout << "✓ range size() operations test passed" << std::endl;
}

void test_get_keys_all() {
    std::cout << "Testing keys() all keys..." << std::endl;

    BinarySearchST<int, std::string> st;

    // Empty table
    std::vector<int> empty_keys = st.keys();
    assert(empty_keys.empty());

    // Add elements in random order
    std::vector<int> input_keys = {5, 2, 8, 1, 9, 3};
    for (int key : input_keys) {
        st.put(key, std::to_string(key));
    }

    // Get all keys (should be sorted)
    std::vector<int> all_keys = st.keys();
    std::vector<int> expected = {1, 2, 3, 5, 8, 9};

    assert(vectors_equal(all_keys, expected));

    std::cout << "✓ keys() all keys test passed" << std::endl;
}

void test_get_keys_range() {
    std::cout << "Testing keys() range operations..." << std::endl;

    BinarySearchST<int, std::string> st;

    // Add elements: 1, 3, 5, 7, 9
    for (int i = 1; i <= 9; i += 2) {
        st.put(i, std::to_string(i));
    }

    // Test various ranges
    std::vector<int> range1 = st.keys(1, 5);
    std::vector<int> expected1 = {1, 3, 5};
    assert(vectors_equal(range1, expected1));

    std::vector<int> range2 = st.keys(2, 8);
    std::vector<int> expected2 = {3, 5, 7};
    assert(vectors_equal(range2, expected2));

    std::vector<int> range3 = st.keys(0, 10);
    std::vector<int> expected3 = {1, 3, 5, 7, 9};
    assert(vectors_equal(range3, expected3));

    // Empty ranges
    std::vector<int> empty_range1 = st.keys(10, 20);
    assert(empty_range1.empty());

    std::vector<int> empty_range2 = st.keys(5, 3);  // lo > hi
    assert(empty_range2.empty());

    std::cout << "✓ keys() range operations test passed" << std::endl;
}

// =============================================================================
// Edge Cases and Error Handling Tests
// =============================================================================

void test_single_element_operations() {
    std::cout << "Testing single element operations..." << std::endl;

    BinarySearchST<int, std::string> st;

    st.put(42, "answer");

    assert(st.size() == 1);
    assert(!st.empty());
    assert(st.contains(42));
    assert(st.get(42) == "answer");
    assert(st.min() == 42);
    assert(st.max() == 42);
    assert(st.select(0) == 42);
    assert(st.rank(42) == 0);
    assert(st.floor(42) == 42);
    assert(st.ceiling(42) == 42);

    std::vector<int> keys = st.keys();
    assert(keys.size() == 1);
    assert(keys[0] == 42);

    st.remove(42);
    assert(st.empty());

    std::cout << "✓ Single element operations test passed" << std::endl;
}

void test_duplicate_key_handling() {
    std::cout << "Testing duplicate key handling..." << std::endl;

    BinarySearchST<int, std::string> st;

    st.put(1, "first");
    st.put(1, "second");
    st.put(1, "third");

    assert(st.size() == 1);  // Should not increase size
    assert(st.get(1) == "third");  // Should have latest value

    std::cout << "✓ Duplicate key handling test passed" << std::endl;
}

void test_string_keys() {
    std::cout << "Testing with string keys..." << std::endl;

    BinarySearchST<std::string, int> st;

    st.put("charlie", 3);
    st.put("alice", 1);
    st.put("bob", 2);
    st.put("david", 4);

    // Should be sorted alphabetically
    assert(st.select(0) == "alice");
    assert(st.select(1) == "bob");
    assert(st.select(2) == "charlie");
    assert(st.select(3) == "david");

    assert(st.min() == "alice");
    assert(st.max() == "david");

    std::cout << "✓ String keys test passed" << std::endl;
}

// =============================================================================
// Performance and Stress Tests
// =============================================================================

void test_large_dataset_performance() {
    std::cout << "Testing large dataset performance..." << std::endl;

    BinarySearchST<int, std::string> st;
    const int num_elements = 1000;

    // Insert elements in reverse order (worst case for insertion)
    for (int i = num_elements - 1; i >= 0; --i) {
        st.put(i, std::to_string(i));
    }

    assert(st.size() == num_elements);

    // Verify all elements are present and in correct order
    for (int i = 0; i < num_elements; ++i) {
        assert(st.contains(i));
        assert(st.get(i) == std::to_string(i));
        assert(st.select(i) == i);
        assert(st.rank(i) == i);
    }

    // Test range operations
    assert(st.size(100, 200) == 101);
    std::vector<int> range = st.keys(100, 110);
    assert(range.size() == 11);

    std::cout << "✓ Large dataset performance test passed" << std::endl;
}

void test_resize_behavior() {
    std::cout << "Testing resize behavior..." << std::endl;

    BinarySearchST<int, std::string> st;

    // Test expansion
    for (int i = 0; i < 20; ++i) {
        st.put(i, std::to_string(i));
    }

    assert(st.size() == 20);

    // Test shrinking
    for (int i = 0; i < 15; ++i) {
        st.remove(i);
    }

    assert(st.size() == 5);

    // Verify remaining elements
    for (int i = 15; i < 20; ++i) {
        assert(st.contains(i));
    }

    std::cout << "✓ Resize behavior test passed" << std::endl;
}

// =============================================================================
// Integration Tests
// =============================================================================

void test_complex_workflow() {
    std::cout << "Testing complex workflow..." << std::endl;

    BinarySearchST<std::string, int> st;

    // Start with empty table
    assert(st.empty());

    // Add several elements
    st.put("elephant", 100);
    st.put("ant", 10);
    st.put("bear", 50);
    st.put("cat", 30);
    st.put("dog", 40);

    // Test ordered operations
    assert(st.min() == "ant");
    assert(st.max() == "elephant");
    assert(st.size() == 5);

    // Update some values
    st.put("cat", 35);  // Update
    st.put("fish", 60); // New

    assert(st.size() == 6);
    assert(st.get("cat") == 35);

    // Range queries
    std::vector<std::string> range = st.keys("bear", "dog");
    std::vector<std::string> expected = {"bear", "cat", "dog"};
    assert(vectors_equal(range, expected));

    // Remove some elements
    st.remove("ant");
    st.removeMax();

    assert(st.size() == 4);
    assert(st.min() == "bear");
    assert(st.max() == "elephant");

    std::cout << "✓ Complex workflow test passed" << std::endl;
}

// =============================================================================
// Main Test Runner
// =============================================================================

void run_all_tests() {
    std::cout << "=== Running Binary Search Symbol Table Tests ===" << std::endl;
    std::cout << std::endl;

    // Constructor and Basic State Tests
    test_default_constructor();
    test_empty_method();
    test_size_method();
    std::cout << std::endl;

    // Put Operation Tests
    test_put_new_keys();
    test_put_update_existing();
    test_put_maintains_order();
    test_put_large_dataset();
    std::cout << std::endl;

    // Get Operation Tests
    test_get_existing_keys();
    test_get_empty_table();
    test_get_nonexistent_keys();
    std::cout << std::endl;

    // Contains Operation Tests
    test_contains_existing_keys();
    test_contains_empty_table();
    test_contains_nonexistent_keys();
    std::cout << std::endl;

    // Remove Operation Tests
    test_remove_existing_keys();
    test_remove_empty_table();
    test_remove_nonexistent_keys();
    test_remove_all_elements();
    std::cout << std::endl;

    // Min/Max Operation Tests
    test_min_max_operations();
    test_remove_min_max();
    std::cout << std::endl;

    // Select and Rank Operation Tests
    test_select_operations();
    test_rank_operations();
    test_select_rank_consistency();
    std::cout << std::endl;

    // Floor and Ceiling Operation Tests
    test_floor_operations();
    test_ceiling_operations();
    std::cout << std::endl;

    // Range Operation Tests
    test_range_size();
    test_get_keys_all();
    test_get_keys_range();
    std::cout << std::endl;

    // Edge Cases and Error Handling Tests
    test_single_element_operations();
    test_duplicate_key_handling();
    test_string_keys();
    std::cout << std::endl;

    // Performance and Stress Tests
    test_large_dataset_performance();
    test_resize_behavior();
    std::cout << std::endl;

    // Integration Tests
    test_complex_workflow();
    std::cout << std::endl;

    std::cout << "=== All Tests Passed Successfully! ===" << std::endl;
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