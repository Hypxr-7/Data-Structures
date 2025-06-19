#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../data_structures/BST.hpp"

/**
 * @brief Test suite for BST using simple assert statements
 *
 * This test suite provides comprehensive testing of the Binary Search Tree
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

// Helper function to check if a vector is sorted
template<typename T>
bool is_sorted(const std::vector<T>& vec) {
    for (size_t i = 1; i < vec.size(); ++i) {
        if (vec[i] < vec[i-1]) return false;
    }
    return true;
}

// =============================================================================
// Constructor and Basic State Tests
// =============================================================================

void test_default_constructor() {
    std::cout << "Testing default constructor..." << std::endl;

    BST<int, std::string> bst_int;
    BST<std::string, int> bst_string;

    // Should create empty BSTs
    assert(bst_int.empty());
    assert(bst_int.size() == 0);
    assert(bst_string.empty());
    assert(bst_string.size() == 0);

    std::cout << "✓ Default constructor test passed" << std::endl;
}

void test_destructor() {
    std::cout << "Testing destructor..." << std::endl;

    // Test with scope to trigger destructor
    {
        BST<int, std::string> bst;
        bst.put(1, "one");
        bst.put(2, "two");
        bst.put(3, "three");
        assert(bst.size() == 3);
    } // Destructor should be called here

    // If we reach here without crashes, destructor worked
    std::cout << "✓ Destructor test passed" << std::endl;
}

void test_empty_method() {
    std::cout << "Testing empty() method..." << std::endl;

    BST<int, std::string> bst;

    // Initially empty
    assert(bst.empty());

    // After adding element
    bst.put(1, "test");
    assert(!bst.empty());

    // After removing element
    bst.remove(1);
    assert(bst.empty());

    std::cout << "✓ empty() method test passed" << std::endl;
}

void test_size_method() {
    std::cout << "Testing size() method..." << std::endl;

    BST<int, std::string> bst;

    assert(bst.size() == 0);

    // Add elements
    bst.put(3, "three");
    assert(bst.size() == 1);

    bst.put(1, "one");
    assert(bst.size() == 2);

    bst.put(2, "two");
    assert(bst.size() == 3);

    // Remove element
    bst.remove(2);
    assert(bst.size() == 2);

    // Remove all elements
    bst.remove(1);
    bst.remove(3);
    assert(bst.size() == 0);

    std::cout << "✓ size() method test passed" << std::endl;
}

// =============================================================================
// Put Operation Tests
// =============================================================================

void test_put_new_keys() {
    std::cout << "Testing put() with new keys..." << std::endl;

    BST<int, std::string> bst;

    // Test adding new keys (BST should maintain order property)
    bst.put(3, "three");
    assert(bst.size() == 1);
    assert(!bst.empty());
    assert(bst.contains(3));

    bst.put(1, "one");
    assert(bst.size() == 2);
    assert(bst.contains(1));

    bst.put(2, "two");
    assert(bst.size() == 3);
    assert(bst.contains(2));

    // Verify BST property through in-order traversal
    std::vector<int> keys = bst.keys();
    assert(is_sorted(keys));

    std::cout << "✓ put() new keys test passed" << std::endl;
}

void test_put_update_existing() {
    std::cout << "Testing put() updates existing keys..." << std::endl;

    BST<int, std::string> bst;

    // Add initial key-value pair
    bst.put(1, "one");
    assert(bst.size() == 1);

    // Update same key with new value
    bst.put(1, "ONE");
    assert(bst.size() == 1);  // Size should not change
    assert(bst.get(1) == "ONE");  // Value should be updated

    // Update again
    bst.put(1, "updated");
    assert(bst.size() == 1);
    assert(bst.get(1) == "updated");

    std::cout << "✓ put() update existing test passed" << std::endl;
}

void test_put_maintains_bst_property() {
    std::cout << "Testing put() maintains BST property..." << std::endl;

    BST<int, std::string> bst;

    // Add keys in various orders
    std::vector<int> keys = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    for (int key : keys) {
        bst.put(key, std::to_string(key));
    }

    assert(bst.size() == 9);

    // Verify BST property through in-order traversal (should be sorted)
    std::vector<int> inorder_keys = bst.keys();
    assert(is_sorted(inorder_keys));

    // Verify all keys are present
    for (int key : keys) {
        assert(bst.contains(key));
        assert(bst.get(key) == std::to_string(key));
    }

    std::cout << "✓ put() maintains BST property test passed" << std::endl;
}

void test_put_large_dataset() {
    std::cout << "Testing put() with large dataset..." << std::endl;

    BST<int, std::string> bst;

    // Add many elements
    const int num_elements = 100;
    for (int i = 0; i < num_elements; ++i) {
        bst.put(i, std::to_string(i));
    }

    assert(bst.size() == num_elements);

    // Verify all elements are present
    for (int i = 0; i < num_elements; ++i) {
        assert(bst.contains(i));
        assert(bst.get(i) == std::to_string(i));
    }

    // Verify BST property
    std::vector<int> keys = bst.keys();
    assert(is_sorted(keys));
    assert(keys.size() == num_elements);

    std::cout << "✓ put() large dataset test passed" << std::endl;
}

// =============================================================================
// Get Operation Tests
// =============================================================================

void test_get_existing_keys() {
    std::cout << "Testing get() with existing keys..." << std::endl;

    BST<int, std::string> bst;

    // Add test data
    bst.put(2, "two");
    bst.put(1, "one");
    bst.put(3, "three");

    // Test getting existing keys
    assert(bst.get(1) == "one");
    assert(bst.get(2) == "two");
    assert(bst.get(3) == "three");

    std::cout << "✓ get() existing keys test passed" << std::endl;
}

void test_get_nonexistent_keys() {
    std::cout << "Testing get() with non-existent keys..." << std::endl;

    BST<int, std::string> bst;

    // Empty BST
    assert(throws_specific_exception<std::invalid_argument>([&bst]() { bst.get(1); }));

    // Populated BST but key doesn't exist
    bst.put(1, "one");
    bst.put(3, "three");

    assert(throws_specific_exception<std::invalid_argument>([&bst]() { bst.get(2); }));
    assert(throws_specific_exception<std::invalid_argument>([&bst]() { bst.get(10); }));

    std::cout << "✓ get() non-existent keys test passed" << std::endl;
}

// =============================================================================
// Contains Operation Tests
// =============================================================================

void test_contains_operations() {
    std::cout << "Testing contains() operations..." << std::endl;

    BST<int, std::string> bst;

    // Empty BST
    assert(!bst.contains(1));

    // Add test data
    bst.put(5, "five");
    bst.put(2, "two");
    bst.put(8, "eight");
    bst.put(1, "one");
    bst.put(3, "three");

    // Test existing keys
    assert(bst.contains(1));
    assert(bst.contains(2));
    assert(bst.contains(3));
    assert(bst.contains(5));
    assert(bst.contains(8));

    // Test non-existing keys
    assert(!bst.contains(0));
    assert(!bst.contains(4));
    assert(!bst.contains(6));
    assert(!bst.contains(10));

    std::cout << "✓ contains() operations test passed" << std::endl;
}

// =============================================================================
// Remove Operation Tests
// =============================================================================

void test_remove_leaf_nodes() {
    std::cout << "Testing remove() leaf nodes..." << std::endl;

    BST<int, std::string> bst;

    // Create a simple BST
    bst.put(3, "three");
    bst.put(1, "one");
    bst.put(5, "five");

    // Remove leaf node
    bst.remove(1);
    assert(bst.size() == 2);
    assert(!bst.contains(1));
    assert(bst.contains(3));
    assert(bst.contains(5));

    std::cout << "✓ remove() leaf nodes test passed" << std::endl;
}

void test_remove_one_child_nodes() {
    std::cout << "Testing remove() nodes with one child..." << std::endl;

    BST<int, std::string> bst;

    // Create BST where some nodes have only one child
    bst.put(3, "three");
    bst.put(1, "one");
    bst.put(2, "two");

    // Remove node with one child
    bst.remove(1);  // Node 1 has only right child (2)
    assert(bst.size() == 2);
    assert(!bst.contains(1));
    assert(bst.contains(2));
    assert(bst.contains(3));

    // Verify BST property is maintained
    std::vector<int> keys = bst.keys();
    assert(is_sorted(keys));

    std::cout << "✓ remove() one child nodes test passed" << std::endl;
}

void test_remove_two_children_nodes() {
    std::cout << "Testing remove() nodes with two children..." << std::endl;

    BST<int, std::string> bst;

    // Create BST with node having two children
    bst.put(5, "five");
    bst.put(3, "three");
    bst.put(7, "seven");
    bst.put(2, "two");
    bst.put(4, "four");
    bst.put(6, "six");
    bst.put(8, "eight");

    // Remove node with two children
    bst.remove(3);  // Node 3 has both left (2) and right (4) children
    assert(bst.size() == 6);
    assert(!bst.contains(3));

    // Verify all other nodes are still present
    assert(bst.contains(2));
    assert(bst.contains(4));
    assert(bst.contains(5));
    assert(bst.contains(6));
    assert(bst.contains(7));
    assert(bst.contains(8));

    // Verify BST property is maintained
    std::vector<int> keys = bst.keys();
    assert(is_sorted(keys));

    std::cout << "✓ remove() two children nodes test passed" << std::endl;
}

void test_remove_root_node() {
    std::cout << "Testing remove() root node..." << std::endl;

    BST<int, std::string> bst;

    // Single node BST
    bst.put(5, "five");
    bst.remove(5);
    assert(bst.empty());

    // Multi-node BST, remove root
    bst.put(5, "five");
    bst.put(3, "three");
    bst.put(7, "seven");
    bst.put(1, "one");
    bst.put(9, "nine");

    bst.remove(5);  // Remove root
    assert(bst.size() == 4);
    assert(!bst.contains(5));

    // Verify remaining nodes
    assert(bst.contains(1));
    assert(bst.contains(3));
    assert(bst.contains(7));
    assert(bst.contains(9));

    // Verify BST property
    std::vector<int> keys = bst.keys();
    assert(is_sorted(keys));

    std::cout << "✓ remove() root node test passed" << std::endl;
}

void test_remove_nonexistent_keys() {
    std::cout << "Testing remove() with non-existent keys..." << std::endl;

    BST<int, std::string> bst;

    // Empty BST
    assert(throws_specific_exception<std::invalid_argument>([&bst]() { bst.remove(1); }));

    // Populated BST but key doesn't exist
    bst.put(1, "one");
    bst.put(3, "three");

    assert(throws_specific_exception<std::invalid_argument>([&bst]() { bst.remove(2); }));
    assert(throws_specific_exception<std::invalid_argument>([&bst]() { bst.remove(10); }));

    // Size should be unchanged
    assert(bst.size() == 2);

    std::cout << "✓ remove() non-existent keys test passed" << std::endl;
}

// =============================================================================
// Min/Max Operation Tests
// =============================================================================

void test_min_max_operations() {
    std::cout << "Testing min() and max() operations..." << std::endl;

    BST<int, std::string> bst;

    // Empty BST should throw
    assert(throws_specific_exception<std::underflow_error>([&bst]() { bst.min(); }));
    assert(throws_specific_exception<std::underflow_error>([&bst]() { bst.max(); }));

    // Add elements
    bst.put(5, "five");
    bst.put(2, "two");
    bst.put(8, "eight");
    bst.put(1, "one");
    bst.put(9, "nine");

    assert(bst.min() == 1);
    assert(bst.max() == 9);

    // Remove min and max
    bst.remove(1);
    bst.remove(9);

    assert(bst.min() == 2);
    assert(bst.max() == 8);

    std::cout << "✓ min() and max() operations test passed" << std::endl;
}

void test_remove_min_max() {
    std::cout << "Testing removeMin() and removeMax()..." << std::endl;

    BST<int, std::string> bst;

    // Empty BST should throw
    assert(throws_specific_exception<std::underflow_error>([&bst]() { bst.removeMin(); }));
    assert(throws_specific_exception<std::underflow_error>([&bst]() { bst.removeMax(); }));

    // Add elements
    bst.put(5, "five");
    bst.put(2, "two");
    bst.put(8, "eight");
    bst.put(1, "one");
    bst.put(9, "nine");

    // Remove min
    bst.removeMin();
    assert(bst.size() == 4);
    assert(bst.min() == 2);
    assert(!bst.contains(1));

    // Remove max
    bst.removeMax();
    assert(bst.size() == 3);
    assert(bst.max() == 8);
    assert(!bst.contains(9));

    // Verify BST property
    std::vector<int> keys = bst.keys();
    assert(is_sorted(keys));

    std::cout << "✓ removeMin() and removeMax() test passed" << std::endl;
}

// =============================================================================
// Select and Rank Operation Tests
// =============================================================================

void test_select_operations() {
    std::cout << "Testing select() operations..." << std::endl;

    BST<int, std::string> bst;

    // Add elements: 1, 3, 5, 7, 9
    for (int i = 1; i <= 9; i += 2) {
        bst.put(i, std::to_string(i));
    }

    // Test select operations
    assert(bst.select(0) == 1);  // 0th element (smallest)
    assert(bst.select(1) == 3);  // 1st element
    assert(bst.select(2) == 5);  // 2nd element
    assert(bst.select(3) == 7);  // 3rd element
    assert(bst.select(4) == 9);  // 4th element (largest)

    // Test bounds
    assert(throws_specific_exception<std::invalid_argument>([&bst]() { bst.select(-1); }));
    assert(throws_specific_exception<std::invalid_argument>([&bst]() { bst.select(bst.size()); }));

    std::cout << "✓ select() operations test passed" << std::endl;
}

void test_rank_operations() {
    std::cout << "Testing rank() operations..." << std::endl;

    BST<int, std::string> bst;

    // Add elements: 1, 3, 5, 7, 9
    for (int i = 1; i <= 9; i += 2) {
        bst.put(i, std::to_string(i));
    }

    // Test rank of existing keys
    assert(bst.rank(1) == 0);
    assert(bst.rank(3) == 1);
    assert(bst.rank(5) == 2);
    assert(bst.rank(7) == 3);
    assert(bst.rank(9) == 4);

    // Test rank of non-existing keys (should return insertion point)
    assert(bst.rank(0) == 0);  // Would be inserted at beginning
    assert(bst.rank(2) == 1);  // Would be inserted between 1 and 3
    assert(bst.rank(4) == 2);  // Would be inserted between 3 and 5
    assert(bst.rank(10) == 5); // Would be inserted at end

    std::cout << "✓ rank() operations test passed" << std::endl;
}

void test_select_rank_consistency() {
    std::cout << "Testing select() and rank() consistency..." << std::endl;

    BST<int, std::string> bst;

    // Add test data
    std::vector<int> keys = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    for (int key : keys) {
        bst.put(key, std::to_string(key));
    }

    // Test select(rank(key)) == key for existing keys
    for (int key : keys) {
        assert(bst.select(bst.rank(key)) == key);
    }

    // Test rank(select(i)) == i for all valid indices
    for (int i = 0; i < bst.size(); ++i) {
        assert(bst.rank(bst.select(i)) == i);
    }

    std::cout << "✓ select() and rank() consistency test passed" << std::endl;
}

// =============================================================================
// Floor and Ceiling Operation Tests
// =============================================================================

void test_floor_operations() {
    std::cout << "Testing floor() operations..." << std::endl;

    BST<int, std::string> bst;

    // Empty BST should throw
    assert(throws_specific_exception<std::underflow_error>([&bst]() { bst.floor(5); }));

    // Add elements: 2, 4, 6, 8, 10
    for (int i = 2; i <= 10; i += 2) {
        bst.put(i, std::to_string(i));
    }

    // Test floor operations
    assert(bst.floor(2) == 2);   // Exact match
    assert(bst.floor(3) == 2);   // Floor of 3 is 2
    assert(bst.floor(5) == 4);   // Floor of 5 is 4
    assert(bst.floor(10) == 10); // Exact match
    assert(bst.floor(15) == 10); // Floor of 15 is 10

    // Key too small should throw
    assert(throws_specific_exception<std::invalid_argument>([&bst]() { bst.floor(1); }));

    std::cout << "✓ floor() operations test passed" << std::endl;
}

void test_ceiling_operations() {
    std::cout << "Testing ceiling() operations..." << std::endl;

    BST<int, std::string> bst;

    // Empty BST should throw
    assert(throws_specific_exception<std::underflow_error>([&bst]() { bst.ceiling(5); }));

    // Add elements: 2, 4, 6, 8, 10
    for (int i = 2; i <= 10; i += 2) {
        bst.put(i, std::to_string(i));
    }

    // Test ceiling operations
    assert(bst.ceiling(2) == 2);   // Exact match
    assert(bst.ceiling(1) == 2);   // Ceiling of 1 is 2
    assert(bst.ceiling(3) == 4);   // Ceiling of 3 is 4
    assert(bst.ceiling(5) == 6);   // Ceiling of 5 is 6
    assert(bst.ceiling(10) == 10); // Exact match

    // Key too large should throw
    assert(throws_specific_exception<std::invalid_argument>([&bst]() { bst.ceiling(15); }));

    std::cout << "✓ ceiling() operations test passed" << std::endl;
}

// =============================================================================
// Range Operation Tests
// =============================================================================

void test_range_size() {
    std::cout << "Testing range size() operations..." << std::endl;

    BST<int, std::string> bst;

    // Empty BST
    assert(bst.size(1, 5) == 0);

    // Add elements: 1, 3, 5, 7, 9
    for (int i = 1; i <= 9; i += 2) {
        bst.put(i, std::to_string(i));
    }

    // Test range sizes
    assert(bst.size(1, 9) == 5);   // All elements
    assert(bst.size(1, 5) == 3);   // 1, 3, 5
    assert(bst.size(3, 7) == 3);   // 3, 5, 7
    assert(bst.size(2, 8) == 3);   // 3, 5, 7
    assert(bst.size(0, 10) == 5);  // All elements
    assert(bst.size(10, 20) == 0); // No elements
    assert(bst.size(5, 3) == 0);   // Invalid range (lo > hi)

    std::cout << "✓ range size() operations test passed" << std::endl;
}

void test_keys_all() {
    std::cout << "Testing keys() all keys..." << std::endl;

    BST<int, std::string> bst;

    // Empty BST
    std::vector<int> empty_keys = bst.keys();
    assert(empty_keys.empty());

    // Add elements in random order
    std::vector<int> input_keys = {5, 2, 8, 1, 9, 3};
    for (int key : input_keys) {
        bst.put(key, std::to_string(key));
    }

    // Get all keys (should be sorted)
    std::vector<int> all_keys = bst.keys();
    std::vector<int> expected = {1, 2, 3, 5, 8, 9};

    assert(vectors_equal(all_keys, expected));
    assert(is_sorted(all_keys));

    std::cout << "✓ keys() all keys test passed" << std::endl;
}

void test_keys_range() {
    std::cout << "Testing keys() range operations..." << std::endl;

    BST<int, std::string> bst;

    // Add elements: 1, 3, 5, 7, 9
    for (int i = 1; i <= 9; i += 2) {
        bst.put(i, std::to_string(i));
    }

    // Test various ranges
    std::vector<int> range1 = bst.keys(1, 5);
    std::vector<int> expected1 = {1, 3, 5};
    assert(vectors_equal(range1, expected1));

    std::vector<int> range2 = bst.keys(2, 8);
    std::vector<int> expected2 = {3, 5, 7};
    assert(vectors_equal(range2, expected2));

    std::vector<int> range3 = bst.keys(0, 10);
    std::vector<int> expected3 = {1, 3, 5, 7, 9};
    assert(vectors_equal(range3, expected3));

    // Empty ranges
    std::vector<int> empty_range1 = bst.keys(10, 20);
    assert(empty_range1.empty());

    std::vector<int> empty_range2 = bst.keys(5, 3);  // lo > hi
    assert(empty_range2.empty());

    std::cout << "✓ keys() range operations test passed" << std::endl;
}

// =============================================================================
// Tree Structure Tests
// =============================================================================

void test_height_operations() {
    std::cout << "Testing height() operations..." << std::endl;

    BST<int, std::string> bst;

    // Empty tree height
    assert(bst.height() == -1);

    // Single node
    bst.put(5, "five");
    assert(bst.height() == 0);

    // Balanced tree
    bst.put(3, "three");
    bst.put(7, "seven");
    assert(bst.height() == 1);

    bst.put(1, "one");
    bst.put(4, "four");
    bst.put(6, "six");
    bst.put(9, "nine");
    assert(bst.height() == 2);

    // Unbalanced tree (linear)
    BST<int, std::string> linear_bst;
    for (int i = 1; i <= 5; ++i) {
        linear_bst.put(i, std::to_string(i));
    }
    assert(linear_bst.height() == 4);  // Linear tree has height n-1

    std::cout << "✓ height() operations test passed" << std::endl;
}

void test_level_order_traversal() {
    std::cout << "Testing levelOrder() operations..." << std::endl;

    BST<int, std::string> bst;

    // Empty tree
    std::vector<int> empty_level = bst.levelOrder();
    assert(empty_level.empty());

    // Build a specific tree structure
    //       5
    //      / \
    //     3   7
    //    / \   \
    //   1   4   9
    bst.put(5, "five");
    bst.put(3, "three");
    bst.put(7, "seven");
    bst.put(1, "one");
    bst.put(4, "four");
    bst.put(9, "nine");

    std::vector<int> level_order = bst.levelOrder();
    std::vector<int> expected = {5, 3, 7, 1, 4, 9};

    assert(vectors_equal(level_order, expected));

    std::cout << "✓ levelOrder() operations test passed" << std::endl;
}

// =============================================================================
// Edge Cases and Error Handling Tests
// =============================================================================

void test_single_element_operations() {
    std::cout << "Testing single element operations..." << std::endl;

    BST<int, std::string> bst;

    bst.put(42, "answer");

    assert(bst.size() == 1);
    assert(!bst.empty());
    assert(bst.contains(42));
    assert(bst.get(42) == "answer");
    assert(bst.min() == 42);
    assert(bst.max() == 42);
    assert(bst.select(0) == 42);
    assert(bst.rank(42) == 0);
    assert(bst.floor(42) == 42);
    assert(bst.ceiling(42) == 42);
    assert(bst.height() == 0);

    std::vector<int> keys = bst.keys();
    assert(keys.size() == 1);
    assert(keys[0] == 42);

    bst.remove(42);
    assert(bst.empty());

    std::cout << "✓ Single element operations test passed" << std::endl;
}

void test_duplicate_key_handling() {
    std::cout << "Testing duplicate key handling..." << std::endl;

    BST<int, std::string> bst;

    bst.put(1, "first");
    bst.put(1, "second");
    bst.put(1, "third");

    assert(bst.size() == 1);  // Should not increase size
    assert(bst.get(1) == "third");  // Should have latest value

    std::cout << "✓ Duplicate key handling test passed" << std::endl;
}

void test_string_keys() {
    std::cout << "Testing with string keys..." << std::endl;

    BST<std::string, int> bst;

    bst.put("charlie", 3);
    bst.put("alice", 1);
    bst.put("bob", 2);
    bst.put("david", 4);

    // Should be sorted alphabetically
    std::vector<std::string> keys = bst.keys();
    assert(is_sorted(keys));

    assert(bst.min() == "alice");
    assert(bst.max() == "david");
    assert(bst.select(0) == "alice");
    assert(bst.select(3) == "david");

    std::cout << "✓ String keys test passed" << std::endl;
}

// =============================================================================
// Performance and Stress Tests
// =============================================================================

void test_large_dataset_performance() {
    std::cout << "Testing large dataset performance..." << std::endl;

    BST<int, std::string> bst;
    const int num_elements = 1000;

    // Insert elements in random order
    std::vector<int> keys(num_elements);
    for (int i = 0; i < num_elements; ++i) {
        keys[i] = i;
    }
    std::random_shuffle(keys.begin(), keys.end());

    for (int key : keys) {
        bst.put(key, std::to_string(key));
    }

    assert(bst.size() == num_elements);

    // Verify all elements are present
    for (int i = 0; i < num_elements; ++i) {
        assert(bst.contains(i));
        assert(bst.get(i) == std::to_string(i));
    }

    // Test range operations
    assert(bst.size(100, 200) == 101);
    std::vector<int> range = bst.keys(100, 110);
    assert(range.size() == 11);
    assert(is_sorted(range));

    std::cout << "✓ Large dataset performance test passed" << std::endl;
}

void test_worst_case_performance() {
    std::cout << "Testing worst case performance (linear tree)..." << std::endl;

    BST<int, std::string> bst;

    // Insert elements in sorted order (creates linear tree)
    const int num_elements = 100;
    for (int i = 0; i < num_elements; ++i) {
        bst.put(i, std::to_string(i));
    }

    assert(bst.size() == num_elements);
    assert(bst.height() == num_elements - 1);  // Linear tree

    // Operations should still work correctly
    assert(bst.min() == 0);
    assert(bst.max() == num_elements - 1);

    for (int i = 0; i < num_elements; ++i) {
        assert(bst.contains(i));
        assert(bst.select(i) == i);
        assert(bst.rank(i) == i);
    }

    std::cout << "✓ Worst case performance test passed" << std::endl;
}

// =============================================================================
// Integration Tests
// =============================================================================

void test_complex_workflow() {
    std::cout << "Testing complex workflow..." << std::endl;

    BST<std::string, int> bst;

    // Start with empty BST
    assert(bst.empty());

    // Add several elements
    bst.put("elephant", 100);
    bst.put("ant", 10);
    bst.put("bear", 50);
    bst.put("cat", 30);
    bst.put("dog", 40);

    // Test ordered operations
    assert(bst.min() == "ant");
    assert(bst.max() == "elephant");
    assert(bst.size() == 5);

    // Update some values
    bst.put("cat", 35);  // Update
    bst.put("fish", 60); // New

    assert(bst.size() == 6);
    assert(bst.get("cat") == 35);

    // Range queries
    std::vector<std::string> range = bst.keys("bear", "dog");
    std::vector<std::string> expected = {"bear", "cat", "dog"};
    assert(vectors_equal(range, expected));

    // Remove some elements
    bst.remove("ant");
    bst.removeMax();

    assert(bst.size() == 4);
    assert(bst.min() == "bear");
    assert(bst.max() == "elephant");

    // Verify BST property is maintained
    std::vector<std::string> final_keys = bst.keys();
    assert(is_sorted(final_keys));

    std::cout << "✓ Complex workflow test passed" << std::endl;
}

void test_remove_all_elements() {
    std::cout << "Testing remove all elements..." << std::endl;

    BST<int, std::string> bst;

    // Add elements
    std::vector<int> keys = {5, 3, 7, 1, 4, 6, 9};
    for (int key : keys) {
        bst.put(key, std::to_string(key));
    }

    // Remove all elements one by one
    for (int key : keys) {
        int size_before = bst.size();
        bst.remove(key);
        assert(bst.size() == size_before - 1);
        assert(!bst.contains(key));

        // Verify BST property is maintained
        if (!bst.empty()) {
            std::vector<int> remaining_keys = bst.keys();
            assert(is_sorted(remaining_keys));
        }
    }

    assert(bst.empty());
    assert(bst.size() == 0);

    std::cout << "✓ Remove all elements test passed" << std::endl;
}

// =============================================================================
// Main Test Runner
// =============================================================================

void run_all_tests() {
    std::cout << "=== Running Binary Search Tree Tests ===" << std::endl;
    std::cout << std::endl;

    // Constructor and Basic State Tests
    test_default_constructor();
    test_destructor();
    test_empty_method();
    test_size_method();
    std::cout << std::endl;

    // Put Operation Tests
    test_put_new_keys();
    test_put_update_existing();
    test_put_maintains_bst_property();
    test_put_large_dataset();
    std::cout << std::endl;

    // Get Operation Tests
    test_get_existing_keys();
    test_get_nonexistent_keys();
    std::cout << std::endl;

    // Contains Operation Tests
    test_contains_operations();
    std::cout << std::endl;

    // Remove Operation Tests
    test_remove_leaf_nodes();
    test_remove_one_child_nodes();
    test_remove_two_children_nodes();
    test_remove_root_node();
    test_remove_nonexistent_keys();
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
    test_keys_all();
    test_keys_range();
    std::cout << std::endl;

    // Tree Structure Tests
    test_height_operations();
    test_level_order_traversal();
    std::cout << std::endl;

    // Edge Cases and Error Handling Tests
    test_single_element_operations();
    test_duplicate_key_handling();
    test_string_keys();
    std::cout << std::endl;

    // Performance and Stress Tests
    test_large_dataset_performance();
    test_worst_case_performance();
    std::cout << std::endl;

    // Integration Tests
    test_complex_workflow();
    test_remove_all_elements();
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