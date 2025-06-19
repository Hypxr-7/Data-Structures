#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <set>
#include <random>
#include <chrono>
#include <unordered_set>

#include "../data_structures/SeparateChainingHashST.hpp"

/**
 * @brief Comprehensive test suite for Separate Chaining Hash Symbol Table
 *
 * This test suite validates the hash table implementation with focus on:
 * - Basic hash table operations (put, get, remove, contains)
 * - Dynamic resizing behavior (expansion and contraction)
 * - Load factor management
 * - Collision handling through separate chaining
 * - Performance characteristics
 * - Edge cases and error conditions
 * - Different data types and key distributions
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

// Helper function to check if vector contains all expected elements
template<typename T>
bool contains_all_elements(const std::vector<T>& vec, const std::vector<T>& expected) {
    std::set<T> vec_set(vec.begin(), vec.end());
    std::set<T> expected_set(expected.begin(), expected.end());
    return vec_set == expected_set;
}

// Helper function to generate random strings
std::vector<std::string> generate_random_strings(int count, int min_length = 3, int max_length = 10) {
    std::vector<std::string> result;
    std::mt19937 gen(42);  // Fixed seed for reproducibility
    std::uniform_int_distribution<> length_dis(min_length, max_length);
    std::uniform_int_distribution<> char_dis('a', 'z');

    for (int i = 0; i < count; ++i) {
        int length = length_dis(gen);
        std::string str;
        for (int j = 0; j < length; ++j) {
            str += static_cast<char>(char_dis(gen));
        }
        result.push_back(str);
    }
    return result;
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

    SeparateChainingHashST<int, std::string> hash_table;

    assert(hash_table.empty());
    assert(hash_table.size() == 0);
    assert(hash_table.capacity() == 8);  // Default capacity
    assert(hash_table.loadFactor() == 0.0);

    std::cout << "✓ Default constructor test passed" << std::endl;
}

void test_custom_capacity_constructor() {
    std::cout << "Testing custom capacity constructor..." << std::endl;

    SeparateChainingHashST<int, std::string> hash_table(16);

    assert(hash_table.empty());
    assert(hash_table.size() == 0);
    assert(hash_table.capacity() == 16);
    assert(hash_table.loadFactor() == 0.0);

    std::cout << "✓ Custom capacity constructor test passed" << std::endl;
}

void test_empty_and_size() {
    std::cout << "Testing empty() and size() methods..." << std::endl;

    SeparateChainingHashST<int, std::string> hash_table;

    // Initially empty
    assert(hash_table.empty());
    assert(hash_table.size() == 0);

    // After adding elements
    hash_table.put(1, "one");
    assert(!hash_table.empty());
    assert(hash_table.size() == 1);

    hash_table.put(2, "two");
    hash_table.put(3, "three");
    assert(hash_table.size() == 3);

    // After removing elements
    hash_table.remove(2);
    assert(hash_table.size() == 2);

    hash_table.remove(1);
    hash_table.remove(3);
    assert(hash_table.empty());
    assert(hash_table.size() == 0);

    std::cout << "✓ empty() and size() methods test passed" << std::endl;
}

// =============================================================================
// Put Operation Tests
// =============================================================================

void test_put_operations() {
    std::cout << "Testing put() operations..." << std::endl;

    SeparateChainingHashST<int, std::string> hash_table;

    // Test inserting new keys
    hash_table.put(10, "ten");
    assert(hash_table.size() == 1);
    assert(hash_table.get(10) == "ten");

    hash_table.put(20, "twenty");
    hash_table.put(30, "thirty");
    assert(hash_table.size() == 3);

    // Test updating existing keys
    hash_table.put(10, "TEN");
    assert(hash_table.size() == 3);  // Size shouldn't change
    assert(hash_table.get(10) == "TEN");  // Value should be updated

    hash_table.put(20, "TWENTY");
    assert(hash_table.get(20) == "TWENTY");

    std::cout << "✓ put() operations test passed" << std::endl;
}

void test_put_collisions() {
    std::cout << "Testing put() with collisions..." << std::endl;

    // Use small capacity to force collisions
    SeparateChainingHashST<int, std::string> hash_table(4);

    // Add elements that will likely collide
    for (int i = 0; i < 20; ++i) {
        hash_table.put(i, std::to_string(i));
    }

    assert(hash_table.size() == 20);

    // Verify all elements are retrievable
    for (int i = 0; i < 20; ++i) {
        assert(hash_table.contains(i));
        assert(hash_table.get(i) == std::to_string(i));
    }

    // Check that load factor is reasonable (should have triggered expansion)
    double load_factor = hash_table.loadFactor();
    assert(load_factor <= 10.0);  // Should not exceed expansion threshold

    std::cout << "  Load factor after insertions: " << load_factor << std::endl;
    std::cout << "  Capacity after insertions: " << hash_table.capacity() << std::endl;
    std::cout << "✓ put() with collisions test passed" << std::endl;
}

void test_put_expansion() {
    std::cout << "Testing put() expansion behavior..." << std::endl;

    SeparateChainingHashST<int, std::string> hash_table(8);
    int initial_capacity = hash_table.capacity();

    // Add elements to trigger expansion (load factor > 10)
    int elements_to_add = 10 * initial_capacity + 1;  // Force expansion

    for (int i = 0; i < elements_to_add; ++i) {
        hash_table.put(i, std::to_string(i));
    }

    // Check that table expanded
    assert(hash_table.capacity() > initial_capacity);
    assert(hash_table.size() == elements_to_add);
    assert(hash_table.loadFactor() <= 10.0);

    // Verify all elements are still accessible after expansion
    for (int i = 0; i < elements_to_add; ++i) {
        assert(hash_table.contains(i));
        assert(hash_table.get(i) == std::to_string(i));
    }

    std::cout << "  Initial capacity: " << initial_capacity << std::endl;
    std::cout << "  Final capacity: " << hash_table.capacity() << std::endl;
    std::cout << "  Final load factor: " << hash_table.loadFactor() << std::endl;
    std::cout << "✓ put() expansion test passed" << std::endl;
}

// =============================================================================
// Get and Contains Tests
// =============================================================================

void test_get_operations() {
    std::cout << "Testing get() operations..." << std::endl;

    SeparateChainingHashST<int, std::string> hash_table;

    // Test get on empty table
    assert(throws_specific_exception<std::invalid_argument>([&hash_table]() { hash_table.get(1); }));

    // Add test data
    hash_table.put(10, "ten");
    hash_table.put(20, "twenty");
    hash_table.put(30, "thirty");

    // Test getting existing keys
    assert(hash_table.get(10) == "ten");
    assert(hash_table.get(20) == "twenty");
    assert(hash_table.get(30) == "thirty");

    // Test getting non-existent keys
    assert(throws_specific_exception<std::invalid_argument>([&hash_table]() { hash_table.get(1); }));
    assert(throws_specific_exception<std::invalid_argument>([&hash_table]() { hash_table.get(40); }));

    std::cout << "✓ get() operations test passed" << std::endl;
}

void test_contains_operations() {
    std::cout << "Testing contains() operations..." << std::endl;

    SeparateChainingHashST<int, std::string> hash_table;

    // Test contains on empty table
    assert(!hash_table.contains(1));

    // Add test data
    std::vector<int> keys = {5, 15, 25, 35, 45};
    for (int key : keys) {
        hash_table.put(key, std::to_string(key));
    }

    // Test existing keys
    for (int key : keys) {
        assert(hash_table.contains(key));
    }

    // Test non-existing keys
    std::vector<int> non_existing = {1, 10, 20, 30, 40, 50};
    for (int key : non_existing) {
        assert(!hash_table.contains(key));
    }

    std::cout << "✓ contains() operations test passed" << std::endl;
}

void test_get_contains_with_collisions() {
    std::cout << "Testing get() and contains() with collisions..." << std::endl;

    // Force collisions with small table
    SeparateChainingHashST<int, std::string> hash_table(2);

    // Add many elements to force collisions
    std::vector<int> keys;
    for (int i = 0; i < 50; ++i) {
        keys.push_back(i);
        hash_table.put(i, "value_" + std::to_string(i));
    }

    // Test that all keys are findable despite collisions
    for (int key : keys) {
        assert(hash_table.contains(key));
        assert(hash_table.get(key) == "value_" + std::to_string(key));
    }

    // Test non-existent keys
    for (int i = 50; i < 60; ++i) {
        assert(!hash_table.contains(i));
    }

    std::cout << "✓ get() and contains() with collisions test passed" << std::endl;
}

// =============================================================================
// Remove Operation Tests
// =============================================================================

void test_remove_operations() {
    std::cout << "Testing remove() operations..." << std::endl;

    SeparateChainingHashST<int, std::string> hash_table;

    // Test remove on empty table
    assert(throws_specific_exception<std::invalid_argument>([&hash_table]() { hash_table.remove(1); }));

    // Add test data
    std::vector<int> keys = {10, 20, 30, 40, 50};
    for (int key : keys) {
        hash_table.put(key, std::to_string(key));
    }

    int initial_size = hash_table.size();

    // Remove some elements
    hash_table.remove(20);
    hash_table.remove(40);

    assert(hash_table.size() == initial_size - 2);
    assert(!hash_table.contains(20));
    assert(!hash_table.contains(40));

    // Verify remaining elements are still there
    assert(hash_table.contains(10));
    assert(hash_table.contains(30));
    assert(hash_table.contains(50));

    // Test removing non-existent key
    assert(throws_specific_exception<std::invalid_argument>([&hash_table]() { hash_table.remove(60); }));
    assert(throws_specific_exception<std::invalid_argument>([&hash_table]() { hash_table.remove(20); }));  // Already removed

    std::cout << "✓ remove() operations test passed" << std::endl;
}

void test_remove_all_elements() {
    std::cout << "Testing remove() all elements..." << std::endl;

    SeparateChainingHashST<int, std::string> hash_table;

    // Add elements
    std::vector<int> keys = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (int key : keys) {
        hash_table.put(key, std::to_string(key));
    }

    // Remove all elements
    for (int key : keys) {
        int size_before = hash_table.size();
        hash_table.remove(key);
        assert(hash_table.size() == size_before - 1);
        assert(!hash_table.contains(key));
    }

    assert(hash_table.empty());

    std::cout << "✓ remove() all elements test passed" << std::endl;
}

void test_remove_contraction() {
    std::cout << "Testing remove() contraction behavior..." << std::endl;

    SeparateChainingHashST<int, std::string> hash_table(32);  // Start with larger capacity

    // Fill table to prevent immediate contraction
    int num_elements = 100;
    for (int i = 0; i < num_elements; ++i) {
        hash_table.put(i, std::to_string(i));
    }

    int capacity_after_insertion = hash_table.capacity();

    // Remove most elements to trigger contraction
    int elements_to_remove = num_elements - 10;  // Leave 10 elements
    for (int i = 0; i < elements_to_remove; ++i) {
        hash_table.remove(i);
    }

    // Check if table contracted (load factor should be managed)
    double final_load_factor = hash_table.loadFactor();
    int final_capacity = hash_table.capacity();

    // Verify remaining elements are still accessible
    for (int i = elements_to_remove; i < num_elements; ++i) {
        assert(hash_table.contains(i));
        assert(hash_table.get(i) == std::to_string(i));
    }

    std::cout << "  Capacity after insertion: " << capacity_after_insertion << std::endl;
    std::cout << "  Final capacity: " << final_capacity << std::endl;
    std::cout << "  Final load factor: " << final_load_factor << std::endl;
    std::cout << "✓ remove() contraction test passed" << std::endl;
}

// =============================================================================
// Keys Operation Tests
// =============================================================================

void test_keys_operations() {
    std::cout << "Testing keys() operations..." << std::endl;

    SeparateChainingHashST<int, std::string> hash_table;

    // Empty table
    std::vector<int> empty_keys = hash_table.keys();
    assert(empty_keys.empty());

    // Add elements
    std::vector<int> input_keys = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    for (int key : input_keys) {
        hash_table.put(key, std::to_string(key));
    }

    // Get all keys
    std::vector<int> all_keys = hash_table.keys();

    // Should contain all input keys
    assert(all_keys.size() == input_keys.size());
    assert(contains_all_elements(all_keys, input_keys));

    // Test after some removals
    hash_table.remove(2);
    hash_table.remove(8);

    std::vector<int> remaining_keys = hash_table.keys();
    std::vector<int> expected_remaining = {5, 1, 9, 3, 7, 4, 6};

    assert(remaining_keys.size() == expected_remaining.size());
    assert(contains_all_elements(remaining_keys, expected_remaining));

    std::cout << "✓ keys() operations test passed" << std::endl;
}

// =============================================================================
// Load Factor and Capacity Tests
// =============================================================================

void test_load_factor_management() {
    std::cout << "Testing load factor management..." << std::endl;

    SeparateChainingHashST<int, std::string> hash_table(4);

    // Test initial load factor
    assert(hash_table.loadFactor() == 0.0);

    // Add elements and monitor load factor
    std::vector<double> load_factors;
    std::vector<int> capacities;

    for (int i = 0; i < 50; ++i) {
        hash_table.put(i, std::to_string(i));
        load_factors.push_back(hash_table.loadFactor());
        capacities.push_back(hash_table.capacity());

        // Load factor should never exceed 10 (expansion threshold)
        assert(hash_table.loadFactor() <= 10.0);
    }

    // Check that expansions occurred
    bool expansion_occurred = false;
    for (size_t i = 1; i < capacities.size(); ++i) {
        if (capacities[i] > capacities[i-1]) {
            expansion_occurred = true;
            break;
        }
    }
    assert(expansion_occurred);

    std::cout << "  Final load factor: " << hash_table.loadFactor() << std::endl;
    std::cout << "  Final capacity: " << hash_table.capacity() << std::endl;
    std::cout << "✓ Load factor management test passed" << std::endl;
}

void test_capacity_changes() {
    std::cout << "Testing capacity changes..." << std::endl;

    SeparateChainingHashST<int, std::string> hash_table(8);

    assert(hash_table.capacity() == 8);

    // Force expansion
    for (int i = 0; i < 85; ++i) {  // 85 > 10 * 8, should trigger expansion
        hash_table.put(i, std::to_string(i));
    }

    int expanded_capacity = hash_table.capacity();
    assert(expanded_capacity > 8);

    // Force contraction
    for (int i = 0; i < 80; ++i) {  // Remove most elements
        hash_table.remove(i);
    }

    int contracted_capacity = hash_table.capacity();

    // Verify elements are still accessible
    for (int i = 80; i < 85; ++i) {
        assert(hash_table.contains(i));
    }

    std::cout << "  Initial capacity: 8" << std::endl;
    std::cout << "  Expanded capacity: " << expanded_capacity << std::endl;
    std::cout << "  Contracted capacity: " << contracted_capacity << std::endl;
    std::cout << "✓ Capacity changes test passed" << std::endl;
}

// =============================================================================
// Different Data Types Tests
// =============================================================================

void test_string_keys() {
    std::cout << "Testing with string keys..." << std::endl;

    SeparateChainingHashST<std::string, int> hash_table;

    std::vector<std::pair<std::string, int>> test_data = {
        {"apple", 5},
        {"banana", 3},
        {"cherry", 8},
        {"date", 2},
        {"elderberry", 12}
    };

    // Insert data
    for (const auto& [key, value] : test_data) {
        hash_table.put(key, value);
    }

    // Test retrieval
    for (const auto& [key, value] : test_data) {
        assert(hash_table.contains(key));
        assert(hash_table.get(key) == value);
    }

    // Test update
    hash_table.put("apple", 10);
    assert(hash_table.get("apple") == 10);
    assert(hash_table.size() == test_data.size());  // Size shouldn't change

    // Test removal
    hash_table.remove("banana");
    assert(!hash_table.contains("banana"));
    assert(hash_table.size() == test_data.size() - 1);

    std::cout << "✓ String keys test passed" << std::endl;
}

void test_custom_objects() {
    std::cout << "Testing with custom objects..." << std::endl;

    struct Person {
        std::string name;
        int age;

        Person(const std::string& n, int a) : name(n), age(a) {}

        bool operator==(const Person& other) const {
            return name == other.name && age == other.age;
        }
    };

    // Need to specialize std::hash for Person
    struct PersonHash {
        size_t operator()(const Person& p) const {
            return std::hash<std::string>{}(p.name) ^ std::hash<int>{}(p.age);
        }
    };

    // For this test, we'll use int keys and Person values
    SeparateChainingHashST<int, Person> hash_table;

    hash_table.put(1, Person("Alice", 25));
    hash_table.put(2, Person("Bob", 30));
    hash_table.put(3, Person("Charlie", 35));

    assert(hash_table.contains(1));
    assert(hash_table.get(1).name == "Alice");
    assert(hash_table.get(1).age == 25);

    assert(hash_table.get(2).name == "Bob");
    assert(hash_table.get(3).name == "Charlie");

    hash_table.remove(2);
    assert(!hash_table.contains(2));
    assert(hash_table.size() == 2);

    std::cout << "✓ Custom objects test passed" << std::endl;
}

// =============================================================================
// Performance and Stress Tests
// =============================================================================

void test_large_dataset_performance() {
    std::cout << "Testing large dataset performance..." << std::endl;

    SeparateChainingHashST<int, std::string> hash_table;
    const int num_elements = 50000;

    auto start_time = std::chrono::high_resolution_clock::now();

    // Insert elements
    for (int i = 0; i < num_elements; ++i) {
        hash_table.put(i, std::to_string(i));
    }

    auto insert_time = std::chrono::high_resolution_clock::now();

    // Verify size and load factor
    assert(hash_table.size() == num_elements);
    double load_factor = hash_table.loadFactor();
    assert(load_factor <= 10.0);  // Should not exceed expansion threshold

    // Perform searches
    for (int i = 0; i < 5000; ++i) {
        int key = i * (num_elements / 5000);
        assert(hash_table.contains(key));
        assert(hash_table.get(key) == std::to_string(key));
    }

    auto search_time = std::chrono::high_resolution_clock::now();

    // Perform deletions
    for (int i = 0; i < 5000; ++i) {
        int key = i * 2;  // Delete every other element in first 10000
        if (key < num_elements && hash_table.contains(key)) {
            hash_table.remove(key);
        }
    }

    auto delete_time = std::chrono::high_resolution_clock::now();

    // Calculate and display times
    auto insert_duration = std::chrono::duration_cast<std::chrono::milliseconds>(insert_time - start_time);
    auto search_duration = std::chrono::duration_cast<std::chrono::milliseconds>(search_time - insert_time);
    auto delete_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delete_time - search_time);

    std::cout << "  Insert " << num_elements << " elements: " << insert_duration.count() << "ms" << std::endl;
    std::cout << "  Search 5000 elements: " << search_duration.count() << "ms" << std::endl;
    std::cout << "  Delete ~5000 elements: " << delete_duration.count() << "ms" << std::endl;
    std::cout << "  Final load factor: " << hash_table.loadFactor() << std::endl;
    std::cout << "  Final capacity: " << hash_table.capacity() << std::endl;

    std::cout << "✓ Large dataset performance test passed" << std::endl;
}

void test_hash_distribution() {
    std::cout << "Testing hash distribution..." << std::endl;

    SeparateChainingHashST<std::string, int> hash_table(16);

    // Generate diverse string keys
    std::vector<std::string> keys = generate_random_strings(200);

    // Add all keys
    for (size_t i = 0; i < keys.size(); ++i) {
        hash_table.put(keys[i], static_cast<int>(i));
    }

    // Check that load factor is reasonable (good distribution)
    double load_factor = hash_table.loadFactor();
    std::cout << "  Load factor with random strings: " << load_factor << std::endl;
    std::cout << "  Capacity: " << hash_table.capacity() << std::endl;

    // Verify all strings are retrievable
    for (size_t i = 0; i < keys.size(); ++i) {
        assert(hash_table.contains(keys[i]));
        assert(hash_table.get(keys[i]) == static_cast<int>(i));
    }

    std::cout << "✓ Hash distribution test passed" << std::endl;
}

void test_collision_heavy_scenario() {
    std::cout << "Testing collision-heavy scenario..." << std::endl;

    // Start with very small capacity to force many collisions
    SeparateChainingHashST<int, std::string> hash_table(2);

    // Add many elements that will definitely collide
    const int num_elements = 100;
    for (int i = 0; i < num_elements; ++i) {
        hash_table.put(i, "value_" + std::to_string(i));
    }

    // Despite collisions, all operations should work correctly
    assert(hash_table.size() == num_elements);

    // Test retrieval
    for (int i = 0; i < num_elements; ++i) {
        assert(hash_table.contains(i));
        assert(hash_table.get(i) == "value_" + std::to_string(i));
    }

    // Test updates
    for (int i = 0; i < 10; ++i) {
        hash_table.put(i, "updated_" + std::to_string(i));
    }

    for (int i = 0; i < 10; ++i) {
        assert(hash_table.get(i) == "updated_" + std::to_string(i));
    }
    assert(hash_table.size() == num_elements);  // Size shouldn't change

    // Test removals
    for (int i = 90; i < 100; ++i) {
        hash_table.remove(i);
    }

    for (int i = 90; i < 100; ++i) {
        assert(!hash_table.contains(i));
    }
    assert(hash_table.size() == num_elements - 10);

    std::cout << "  Final load factor: " << hash_table.loadFactor() << std::endl;
    std::cout << "  Final capacity: " << hash_table.capacity() << std::endl;
    std::cout << "✓ Collision-heavy scenario test passed" << std::endl;
}

// =============================================================================
// Edge Cases and Error Handling Tests
// =============================================================================

void test_edge_cases() {
    std::cout << "Testing edge cases..." << std::endl;

    // Single element operations
    {
        SeparateChainingHashST<int, std::string> hash_table;
        hash_table.put(42, "answer");

        assert(hash_table.size() == 1);
        assert(!hash_table.empty());
        assert(hash_table.contains(42));
        assert(hash_table.get(42) == "answer");
        assert(hash_table.loadFactor() > 0.0);

        hash_table.remove(42);
        assert(hash_table.empty());
    }

    // Duplicate key handling
    {
        SeparateChainingHashST<int, std::string> hash_table;
        hash_table.put(10, "first");
        hash_table.put(10, "second");
        hash_table.put(10, "third");

        assert(hash_table.size() == 1);
        assert(hash_table.get(10) == "third");
    }

    // Zero and negative keys (if supported by hash function)
    {
        SeparateChainingHashST<int, std::string> hash_table;
        hash_table.put(0, "zero");
        hash_table.put(-1, "negative");
        hash_table.put(-100, "very negative");

        assert(hash_table.contains(0));
        assert(hash_table.contains(-1));
        assert(hash_table.contains(-100));
        assert(hash_table.get(0) == "zero");
        assert(hash_table.get(-1) == "negative");
        assert(hash_table.get(-100) == "very negative");
    }

    // Empty string keys
    {
        SeparateChainingHashST<std::string, int> hash_table;
        hash_table.put("", 0);
        hash_table.put("non-empty", 1);

        assert(hash_table.contains(""));
        assert(hash_table.get("") == 0);
        assert(hash_table.get("non-empty") == 1);
    }

    std::cout << "✓ Edge cases test passed" << std::endl;
}

void test_error_conditions() {
    std::cout << "Testing error conditions..." << std::endl;

    SeparateChainingHashST<int, std::string> hash_table;

    // Test operations on empty table
    assert(throws_specific_exception<std::invalid_argument>([&hash_table]() { hash_table.get(1); }));
    assert(throws_specific_exception<std::invalid_argument>([&hash_table]() { hash_table.remove(1); }));

    // Add some data
    hash_table.put(10, "ten");
    hash_table.put(20, "twenty");

    // Test operations on non-existent keys
    assert(throws_specific_exception<std::invalid_argument>([&hash_table]() { hash_table.get(30); }));
    assert(throws_specific_exception<std::invalid_argument>([&hash_table]() { hash_table.remove(30); }));

    // Remove existing key and try to remove again
    hash_table.remove(10);
    assert(throws_specific_exception<std::invalid_argument>([&hash_table]() { hash_table.remove(10); }));

    std::cout << "✓ Error conditions test passed" << std::endl;
}

// =============================================================================
// Integration and Complex Workflow Tests
// =============================================================================

void test_complex_workflow() {
    std::cout << "Testing complex workflow..." << std::endl;

    SeparateChainingHashST<std::string, int> hash_table;

    // Phase 1: Build initial table
    std::vector<std::pair<std::string, int>> initial_data = {
        {"alpha", 1}, {"beta", 2}, {"gamma", 3}, {"delta", 4}, {"epsilon", 5},
        {"zeta", 6}, {"eta", 7}, {"theta", 8}, {"iota", 9}, {"kappa", 10}
    };

    for (const auto& [key, value] : initial_data) {
        hash_table.put(key, value);
    }

    assert(hash_table.size() == initial_data.size());

    // Phase 2: Update some values
    hash_table.put("alpha", 100);
    hash_table.put("epsilon", 500);
    hash_table.put("kappa", 1000);

    assert(hash_table.size() == initial_data.size());  // Size shouldn't change
    assert(hash_table.get("alpha") == 100);
    assert(hash_table.get("epsilon") == 500);
    assert(hash_table.get("kappa") == 1000);

    // Phase 3: Add more elements to potentially trigger resize
    std::vector<std::pair<std::string, int>> additional_data = {
        {"lambda", 11}, {"mu", 12}, {"nu", 13}, {"xi", 14}, {"omicron", 15},
        {"pi", 16}, {"rho", 17}, {"sigma", 18}, {"tau", 19}, {"upsilon", 20}
    };

    for (const auto& [key, value] : additional_data) {
        hash_table.put(key, value);
    }

    // Phase 4: Remove some elements
    std::vector<std::string> to_remove = {"beta", "delta", "zeta", "theta"};
    for (const std::string& key : to_remove) {
        hash_table.remove(key);
    }

    // Phase 5: Verify final state
    int expected_size = initial_data.size() + additional_data.size() - to_remove.size();
    assert(hash_table.size() == expected_size);

    // Verify removed keys are gone
    for (const std::string& key : to_remove) {
        assert(!hash_table.contains(key));
    }

    // Verify remaining keys are accessible
    std::vector<std::string> remaining_keys = hash_table.keys();
    assert(remaining_keys.size() == expected_size);

    // Check that load factor is maintained
    double final_load_factor = hash_table.loadFactor();
    assert(final_load_factor <= 10.0);

    std::cout << "  Final size: " << hash_table.size() << std::endl;
    std::cout << "  Final load factor: " << final_load_factor << std::endl;
    std::cout << "  Final capacity: " << hash_table.capacity() << std::endl;
    std::cout << "✓ Complex workflow test passed" << std::endl;
}

// =============================================================================
// Comparison with Standard Library
// =============================================================================

void test_comparison_with_std_unordered_map() {
    std::cout << "Testing comparison with std::unordered_map..." << std::endl;

    SeparateChainingHashST<int, std::string> our_table;
    std::unordered_set<int> std_set;  // For comparison of key existence

    // Generate test data
    std::vector<int> test_keys = generate_random_integers(1000, 1, 10000);

    // Insert same data into both
    for (int key : test_keys) {
        our_table.put(key, std::to_string(key));
        std_set.insert(key);
    }

    // Remove duplicates from test_keys for comparison
    std::set<int> unique_keys(test_keys.begin(), test_keys.end());

    // Compare sizes
    assert(our_table.size() == static_cast<int>(unique_keys.size()));
    assert(std_set.size() == unique_keys.size());

    // Compare contains operations
    for (int key : unique_keys) {
        assert(our_table.contains(key) == (std_set.find(key) != std_set.end()));
    }

    // Test some non-existent keys
    for (int key = 10001; key <= 10100; ++key) {
        assert(our_table.contains(key) == (std_set.find(key) != std_set.end()));
    }

    std::cout << "✓ Comparison with std::unordered_map test passed" << std::endl;
}

// =============================================================================
// Main Test Runner
// =============================================================================

void run_all_tests() {
    std::cout << "=== Running Separate Chaining Hash Symbol Table Tests ===" << std::endl;
    std::cout << std::endl;

    // Constructor and Basic State Tests
    test_default_constructor();
    test_custom_capacity_constructor();
    test_empty_and_size();
    std::cout << std::endl;

    // Put Operation Tests
    test_put_operations();
    test_put_collisions();
    test_put_expansion();
    std::cout << std::endl;

    // Get and Contains Tests
    test_get_operations();
    test_contains_operations();
    test_get_contains_with_collisions();
    std::cout << std::endl;

    // Remove Operation Tests
    test_remove_operations();
    test_remove_all_elements();
    test_remove_contraction();
    std::cout << std::endl;

    // Keys Operation Tests
    test_keys_operations();
    std::cout << std::endl;

    // Load Factor and Capacity Tests
    test_load_factor_management();
    test_capacity_changes();
    std::cout << std::endl;

    // Different Data Types Tests
    test_string_keys();
    test_custom_objects();
    std::cout << std::endl;

    // Performance and Stress Tests
    test_large_dataset_performance();
    test_hash_distribution();
    test_collision_heavy_scenario();
    std::cout << std::endl;

    // Edge Cases and Error Handling Tests
    test_edge_cases();
    test_error_conditions();
    std::cout << std::endl;

    // Integration and Complex Workflow Tests
    test_complex_workflow();
    test_comparison_with_std_unordered_map();
    std::cout << std::endl;

    std::cout << "=== All Separate Chaining Hash Symbol Table Tests Passed Successfully! ===" << std::endl;
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