#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include "../data_structures/SequentialSearchST.hpp"  // Assuming the header file name

/**
 * @brief Test suite for SequentialSearchST using simple assert statements
 *
 * This test suite provides comprehensive testing of the Sequential Search Symbol Table
 * using only standard library assert statements. Tests are organized by functionality
 * and include edge cases, error conditions, and integration scenarios.
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

// Helper function to check if vector contains all expected elements (order independent)
template<typename T>
bool contains_all(const std::vector<T>& vec, const std::vector<T>& expected) {
    if (vec.size() != expected.size()) return false;

    std::vector<T> sorted_vec = vec;
    std::vector<T> sorted_expected = expected;
    std::sort(sorted_vec.begin(), sorted_vec.end());
    std::sort(sorted_expected.begin(), sorted_expected.end());

    return sorted_vec == sorted_expected;
}

// =============================================================================
// Constructor and Basic State Tests
// =============================================================================

void test_default_constructor() {
    std::cout << "Testing default constructor..." << std::endl;

    SequentialSearchST<int, std::string> st_int;
    SequentialSearchST<std::string, int> st_string;

    // Should create empty symbol tables
    assert(st_int.empty());
    assert(st_int.size() == 0);
    assert(st_string.empty());
    assert(st_string.size() == 0);

    std::cout << "✓ Default constructor test passed" << std::endl;
}

void test_empty_method() {
    std::cout << "Testing empty() method..." << std::endl;

    SequentialSearchST<int, std::string> st;

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

    SequentialSearchST<int, std::string> st;

    assert(st.size() == 0);

    // Add elements
    st.put(1, "one");
    assert(st.size() == 1);

    st.put(2, "two");
    assert(st.size() == 2);

    st.put(3, "three");
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

    SequentialSearchST<int, std::string> st;

    // Test adding new keys
    st.put(1, "one");
    assert(st.size() == 1);
    assert(!st.empty());

    st.put(2, "two");
    assert(st.size() == 2);

    // Test with different types
    SequentialSearchST<std::string, int> st_string;
    st_string.put("hello", 42);
    assert(st_string.size() == 1);
    assert(!st_string.empty());

    std::cout << "✓ put() new keys test passed" << std::endl;
}

void test_put_update_existing() {
    std::cout << "Testing put() updates existing keys..." << std::endl;

    SequentialSearchST<int, std::string> st;

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

void test_put_multiple_keys() {
    std::cout << "Testing put() with multiple keys..." << std::endl;

    SequentialSearchST<int, std::string> st;

    // Add multiple different keys
    st.put(1, "one");
    st.put(2, "two");
    st.put(3, "three");
    st.put(4, "four");
    st.put(5, "five");

    assert(st.size() == 5);
    assert(st.get(1) == "one");
    assert(st.get(2) == "two");
    assert(st.get(3) == "three");
    assert(st.get(4) == "four");
    assert(st.get(5) == "five");

    std::cout << "✓ put() multiple keys test passed" << std::endl;
}

// =============================================================================
// Get Operation Tests
// =============================================================================

void test_get_existing_keys() {
    std::cout << "Testing get() with existing keys..." << std::endl;

    SequentialSearchST<int, std::string> st;

    // Add test data
    st.put(1, "one");
    st.put(2, "two");
    st.put(3, "three");

    // Test getting existing keys
    assert(st.get(1) == "one");
    assert(st.get(2) == "two");
    assert(st.get(3) == "three");

    // Test with string keys
    SequentialSearchST<std::string, int> st_string;
    st_string.put("apple", 10);
    st_string.put("banana", 20);

    assert(st_string.get("apple") == 10);
    assert(st_string.get("banana") == 20);

    std::cout << "✓ get() existing keys test passed" << std::endl;
}

void test_get_nonexistent_keys() {
    std::cout << "Testing get() with non-existent keys..." << std::endl;

    SequentialSearchST<int, std::string> st;

    // Empty table
    assert(throws_exception([&st]() { st.get(999); }));

    // Populated table but key doesn't exist
    st.put(1, "one");
    st.put(2, "two");
    assert(throws_exception([&st]() { st.get(10); }));

    SequentialSearchST<std::string, int> st_string;
    assert(throws_exception([&st_string]() { st_string.get("nonexistent"); }));

    std::cout << "✓ get() non-existent keys test passed" << std::endl;
}

void test_get_after_updates() {
    std::cout << "Testing get() after updates..." << std::endl;

    SequentialSearchST<int, std::string> st;

    st.put(1, "original");
    assert(st.get(1) == "original");

    st.put(1, "updated");
    assert(st.get(1) == "updated");

    st.put(1, "final");
    assert(st.get(1) == "final");

    std::cout << "✓ get() after updates test passed" << std::endl;
}

// =============================================================================
// Contains Operation Tests
// =============================================================================

void test_contains_existing_keys() {
    std::cout << "Testing contains() with existing keys..." << std::endl;

    SequentialSearchST<int, std::string> st;

    // Add test data
    st.put(1, "one");
    st.put(2, "two");
    st.put(3, "three");

    assert(st.contains(1));
    assert(st.contains(2));
    assert(st.contains(3));

    // Test with string keys
    SequentialSearchST<std::string, int> st_string;
    st_string.put("apple", 10);
    st_string.put("banana", 20);

    assert(st_string.contains("apple"));
    assert(st_string.contains("banana"));

    std::cout << "✓ contains() existing keys test passed" << std::endl;
}

void test_contains_nonexistent_keys() {
    std::cout << "Testing contains() with non-existent keys..." << std::endl;

    SequentialSearchST<int, std::string> st;

    // Empty table
    assert(!st.contains(1));

    // Populated table but key doesn't exist
    st.put(1, "one");
    st.put(2, "two");
    assert(!st.contains(10));
    assert(!st.contains(0));

    SequentialSearchST<std::string, int> st_string;
    st_string.put("apple", 10);
    assert(!st_string.contains("orange"));
    assert(!st_string.contains(""));

    std::cout << "✓ contains() non-existent keys test passed" << std::endl;
}

void test_contains_after_modifications() {
    std::cout << "Testing contains() after modifications..." << std::endl;

    SequentialSearchST<int, std::string> st;

    // Add key
    st.put(42, "answer");
    assert(st.contains(42));

    // Update key (should still contain)
    st.put(42, "updated");
    assert(st.contains(42));

    // Remove key
    st.remove(42);
    assert(!st.contains(42));

    std::cout << "✓ contains() after modifications test passed" << std::endl;
}

// =============================================================================
// Remove Operation Tests
// =============================================================================

void test_remove_existing_keys() {
    std::cout << "Testing remove() with existing keys..." << std::endl;

    SequentialSearchST<int, std::string> st;

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
    assert(throws_exception([&st]() { st.get(3); }));

    // Other elements should still exist
    assert(st.contains(1));
    assert(st.contains(2));
    assert(st.contains(4));
    assert(st.contains(5));

    std::cout << "✓ remove() existing keys test passed" << std::endl;
}

void test_remove_nonexistent_keys() {
    std::cout << "Testing remove() with non-existent keys..." << std::endl;

    SequentialSearchST<int, std::string> st;

    // Empty table
    assert(throws_exception([&st]() { st.remove(999); }));

    // Populated table but key doesn't exist
    st.put(1, "one");
    st.put(2, "two");
    assert(throws_exception([&st]() { st.remove(10); }));

    // Remove existing key, then try to remove again
    st.remove(1);
    assert(throws_exception([&st]() { st.remove(1); }));

    std::cout << "✓ remove() non-existent keys test passed" << std::endl;
}

void test_remove_all_elements() {
    std::cout << "Testing remove() all elements..." << std::endl;

    SequentialSearchST<int, std::string> st;

    // Add test data
    st.put(1, "one");
    st.put(2, "two");
    st.put(3, "three");
    st.put(4, "four");
    st.put(5, "five");

    std::vector<int> keys = {1, 2, 3, 4, 5};

    for (int key : keys) {
        int size_before = st.size();
        st.remove(key);
        assert(st.size() == size_before - 1);
        assert(!st.contains(key));
    }

    assert(st.empty());
    assert(st.size() == 0);

    std::cout << "✓ remove() all elements test passed" << std::endl;
}

void test_remove_single_element() {
    std::cout << "Testing remove() with single element..." << std::endl;

    SequentialSearchST<int, std::string> st;

    st.put(42, "answer");
    assert(st.size() == 1);
    assert(!st.empty());

    st.remove(42);
    assert(st.size() == 0);
    assert(st.empty());
    assert(!st.contains(42));

    std::cout << "✓ remove() single element test passed" << std::endl;
}

// =============================================================================
// Keys Operation Tests
// =============================================================================

void test_keys_returns_all() {
    std::cout << "Testing keys() returns all keys..." << std::endl;

    SequentialSearchST<int, std::string> st;

    // Add test data
    st.put(1, "one");
    st.put(2, "two");
    st.put(3, "three");
    st.put(4, "four");
    st.put(5, "five");

    std::vector<int> keys = st.keys();
    assert(keys.size() == 5);

    // All keys should be present (order may vary)
    std::vector<int> expected = {1, 2, 3, 4, 5};
    assert(contains_all(keys, expected));

    std::cout << "✓ keys() returns all test passed" << std::endl;
}

void test_keys_empty_table() {
    std::cout << "Testing keys() on empty table..." << std::endl;

    SequentialSearchST<int, std::string> st;

    std::vector<int> keys = st.keys();
    assert(keys.empty());
    assert(keys.size() == 0);

    std::cout << "✓ keys() empty table test passed" << std::endl;
}

void test_keys_with_string_keys() {
    std::cout << "Testing keys() with string keys..." << std::endl;

    SequentialSearchST<std::string, int> st;

    st.put("apple", 10);
    st.put("banana", 20);
    st.put("cherry", 30);
    st.put("date", 40);

    std::vector<std::string> keys = st.keys();
    assert(keys.size() == 4);

    std::vector<std::string> expected = {"apple", "banana", "cherry", "date"};
    assert(contains_all(keys, expected));

    std::cout << "✓ keys() with string keys test passed" << std::endl;
}

void test_keys_after_modifications() {
    std::cout << "Testing keys() after modifications..." << std::endl;

    SequentialSearchST<int, std::string> st;

    st.put(1, "one");
    st.put(2, "two");
    st.put(3, "three");

    std::vector<int> keys = st.keys();
    assert(keys.size() == 3);

    // Remove one key
    st.remove(2);
    keys = st.keys();
    assert(keys.size() == 2);

    std::vector<int> expected = {1, 3};
    assert(contains_all(keys, expected));

    std::cout << "✓ keys() after modifications test passed" << std::endl;
}

// =============================================================================
// Edge Cases and Error Handling Tests
// =============================================================================

void test_single_element_operations() {
    std::cout << "Testing single element operations..." << std::endl;

    SequentialSearchST<int, std::string> st;

    st.put(42, "answer");

    assert(st.size() == 1);
    assert(!st.empty());
    assert(st.contains(42));
    assert(st.get(42) == "answer");

    std::vector<int> keys = st.keys();
    assert(keys.size() == 1);
    assert(keys[0] == 42);

    st.remove(42);
    assert(st.empty());
    assert(st.size() == 0);

    std::cout << "✓ Single element operations test passed" << std::endl;
}

void test_duplicate_key_insertions() {
    std::cout << "Testing duplicate key insertions..." << std::endl;

    SequentialSearchST<int, std::string> st;

    st.put(1, "first");
    st.put(1, "second");
    st.put(1, "third");

    assert(st.size() == 1);  // Should not increase size
    assert(st.get(1) == "third");  // Should have latest value

    std::cout << "✓ Duplicate key insertions test passed" << std::endl;
}

void test_empty_string_keys_values() {
    std::cout << "Testing empty string keys and values..." << std::endl;

    SequentialSearchST<std::string, std::string> st;

    st.put("", "empty_key");
    st.put("empty_value", "");
    st.put("", "");  // This should update the empty key

    assert(st.size() == 2);
    assert(st.contains(""));
    assert(st.contains("empty_value"));
    assert(st.get("") == "");
    assert(st.get("empty_value") == "");

    std::cout << "✓ Empty string keys and values test passed" << std::endl;
}

void test_exception_types() {
    std::cout << "Testing exception types..." << std::endl;

    SequentialSearchST<int, std::string> st;

    // Test get() throws std::invalid_argument
    try {
        st.get(999);
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& e) {
        // Expected exception type
        assert(std::string(e.what()) == "Key not in ST");
    } catch (...) {
        assert(false);  // Wrong exception type
    }

    // Test remove() throws std::invalid_argument
    try {
        st.remove(999);
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& e) {
        // Expected exception type
        assert(std::string(e.what()) == "Key not in ST");
    } catch (...) {
        assert(false);  // Wrong exception type
    }

    std::cout << "✓ Exception types test passed" << std::endl;
}

// =============================================================================
// Performance and Stress Tests
// =============================================================================

void test_larger_dataset() {
    std::cout << "Testing larger dataset..." << std::endl;

    SequentialSearchST<int, std::string> st;
    const int num_elements = 100;

    // Insert elements
    for (int i = 0; i < num_elements; ++i) {
        st.put(i, std::to_string(i));
    }

    assert(st.size() == num_elements);
    assert(!st.empty());

    // Verify all elements exist
    for (int i = 0; i < num_elements; ++i) {
        assert(st.contains(i));
        assert(st.get(i) == std::to_string(i));
    }

    // Test keys() with larger dataset
    std::vector<int> keys = st.keys();
    assert(keys.size() == num_elements);

    // Remove half the elements
    for (int i = 0; i < num_elements; i += 2) {
        st.remove(i);
    }

    assert(st.size() == num_elements / 2);

    // Verify correct elements remain
    for (int i = 0; i < num_elements; ++i) {
        if (i % 2 == 0) {
            assert(!st.contains(i));
        } else {
            assert(st.contains(i));
        }
    }

    std::cout << "✓ Larger dataset test passed" << std::endl;
}

void test_value_copy_semantics() {
    std::cout << "Testing value copy semantics..." << std::endl;

    SequentialSearchST<std::string, int> st;

    st.put("test", 42);

    // Get should return a copy, not a reference
    int value = st.get("test");
    value = 999;  // Modify the copy

    // Original value in table should be unchanged
    assert(st.get("test") == 42);

    std::cout << "✓ Value copy semantics test passed" << std::endl;
}

// =============================================================================
// Integration Tests
// =============================================================================

void test_complex_workflow() {
    std::cout << "Testing complex workflow..." << std::endl;

    SequentialSearchST<std::string, int> st;

    // Start with empty table
    assert(st.empty());

    // Add several elements
    st.put("apple", 10);
    st.put("banana", 20);
    st.put("cherry", 30);

    // Update some values
    st.put("apple", 15);  // Update
    st.put("date", 40);   // New

    assert(st.size() == 4);
    assert(st.get("apple") == 15);

    // Remove some elements
    st.remove("banana");
    assert(st.size() == 3);
    assert(!st.contains("banana"));

    // Check final state
    std::vector<std::string> keys = st.keys();
    std::vector<std::string> expected = {"apple", "cherry", "date"};
    assert(contains_all(keys, expected));

    // Verify values
    assert(st.get("apple") == 15);
    assert(st.get("cherry") == 30);
    assert(st.get("date") == 40);

    std::cout << "✓ Complex workflow test passed" << std::endl;
}

// =============================================================================
// Custom Type Tests
// =============================================================================

void test_custom_key_type() {
    std::cout << "Testing custom key type..." << std::endl;

    struct CustomKey {
        int id;
        std::string name;

        bool operator==(const CustomKey& other) const {
            return id == other.id && name == other.name;
        }
    };

    SequentialSearchST<CustomKey, std::string> st;

    CustomKey key1{1, "first"};
    CustomKey key2{2, "second"};
    CustomKey key1_duplicate{1, "first"};

    st.put(key1, "value1");
    st.put(key2, "value2");

    assert(st.size() == 2);
    assert(st.contains(key1));
    assert(st.contains(key1_duplicate));  // Should be equal
    assert(st.get(key1) == "value1");

    // Update with duplicate key
    st.put(key1_duplicate, "updated");
    assert(st.size() == 2);  // Size unchanged
    assert(st.get(key1) == "updated");

    std::cout << "✓ Custom key type test passed" << std::endl;
}

// =============================================================================
// Main Test Runner
// =============================================================================

void run_all_tests() {
    std::cout << "=== Running Sequential Search Symbol Table Tests ===" << std::endl;
    std::cout << std::endl;

    // Constructor and Basic State Tests
    test_default_constructor();
    test_empty_method();
    test_size_method();
    std::cout << std::endl;

    // Put Operation Tests
    test_put_new_keys();
    test_put_update_existing();
    test_put_multiple_keys();
    std::cout << std::endl;

    // Get Operation Tests
    test_get_existing_keys();
    test_get_nonexistent_keys();
    test_get_after_updates();
    std::cout << std::endl;

    // Contains Operation Tests
    test_contains_existing_keys();
    test_contains_nonexistent_keys();
    test_contains_after_modifications();
    std::cout << std::endl;

    // Remove Operation Tests
    test_remove_existing_keys();
    test_remove_nonexistent_keys();
    test_remove_all_elements();
    test_remove_single_element();
    std::cout << std::endl;

    // Keys Operation Tests
    test_keys_returns_all();
    test_keys_empty_table();
    test_keys_with_string_keys();
    test_keys_after_modifications();
    std::cout << std::endl;

    // Edge Cases and Error Handling Tests
    test_single_element_operations();
    test_duplicate_key_insertions();
    test_empty_string_keys_values();
    test_exception_types();
    std::cout << std::endl;

    // Performance and Stress Tests
    test_larger_dataset();
    test_value_copy_semantics();
    std::cout << std::endl;

    // Integration Tests
    test_complex_workflow();
    std::cout << std::endl;

    // Custom Type Tests
    test_custom_key_type();
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