#include <cassert>
#include <iostream>
#include <set>
#include <stdexcept>
#include <vector>

#include "../data_structures/QuickFindUF.hpp"

void test_constructor() {
    QuickFindUF uf(5);

    // Initially, each element should be in its own component
    assert(uf.count() == 5);

    // Each element should be its own root
    for (int i = 0; i < 5; ++i) {
        assert(uf.find(i) == i);
    }

    // No elements should be connected initially
    for (int i = 0; i < 5; ++i) {
        for (int j = i + 1; j < 5; ++j) {
            assert(!uf.connected(i, j));
        }
    }

    std::cout << "✓ Constructor test passed\n";
}

void test_single_element() {
    QuickFindUF uf(1);

    assert(uf.count() == 1);
    assert(uf.find(0) == 0);
    assert(uf.connected(0, 0));  // Element connected to itself

    std::cout << "✓ Single element test passed\n";
}

void test_basic_union() {
    QuickFindUF uf(5);

    // Union 0 and 1
    uf.unite(0, 1);
    assert(uf.count() == 4);
    assert(uf.connected(0, 1));
    assert(uf.find(0) == uf.find(1));

    // Other elements should still be separate
    assert(!uf.connected(0, 2));
    assert(!uf.connected(1, 2));
    assert(!uf.connected(2, 3));

    std::cout << "✓ Basic union test passed\n";
}

void test_multiple_unions() {
    QuickFindUF uf(6);

    // Create two separate components: {0,1,2} and {3,4,5}
    uf.unite(0, 1);
    uf.unite(1, 2);
    assert(uf.count() == 4);

    uf.unite(3, 4);
    uf.unite(4, 5);
    assert(uf.count() == 2);

    // Check connections within components
    assert(uf.connected(0, 1));
    assert(uf.connected(0, 2));
    assert(uf.connected(1, 2));

    assert(uf.connected(3, 4));
    assert(uf.connected(3, 5));
    assert(uf.connected(4, 5));

    // Check no connections between components
    assert(!uf.connected(0, 3));
    assert(!uf.connected(1, 4));
    assert(!uf.connected(2, 5));

    std::cout << "✓ Multiple unions test passed\n";
}

void test_union_large_components() {
    QuickFindUF uf(10);

    // Create component {0,1,2,3,4}
    uf.unite(0, 1);
    uf.unite(1, 2);
    uf.unite(2, 3);
    uf.unite(3, 4);

    // Create component {5,6,7}
    uf.unite(5, 6);
    uf.unite(6, 7);

    assert(uf.count() == 4);  // {0,1,2,3,4}, {5,6,7}, {8}, {9}

    // Union the two large components
    uf.unite(2, 6);
    assert(uf.count() == 3);  // {0,1,2,3,4,5,6,7}, {8}, {9}

    // All elements in the large component should be connected
    std::vector<int> component = {0, 1, 2, 3, 4, 5, 6, 7};
    for (int i : component) {
        for (int j : component) {
            assert(uf.connected(i, j));
        }
    }

    // Separate elements should not be connected
    assert(!uf.connected(0, 8));
    assert(!uf.connected(5, 9));

    std::cout << "✓ Union large components test passed\n";
}

void test_redundant_unions() {
    QuickFindUF uf(4);

    uf.unite(0, 1);
    int count_after_first = uf.count();

    // Union the same elements again - should have no effect
    uf.unite(0, 1);
    assert(uf.count() == count_after_first);

    uf.unite(1, 0);  // Reverse order - should also have no effect
    assert(uf.count() == count_after_first);

    std::cout << "✓ Redundant unions test passed\n";
}

void test_self_union() {
    QuickFindUF uf(3);

    int initial_count = uf.count();

    // Union element with itself - should have no effect
    uf.unite(0, 0);
    assert(uf.count() == initial_count);

    uf.unite(1, 1);
    assert(uf.count() == initial_count);

    std::cout << "✓ Self union test passed\n";
}

void test_find_consistency() {
    QuickFindUF uf(5);

    uf.unite(0, 2);
    uf.unite(1, 3);

    // Elements in the same component should have the same find result
    assert(uf.find(0) == uf.find(2));
    assert(uf.find(1) == uf.find(3));

    // Elements in different components should have different find results
    assert(uf.find(0) != uf.find(1));
    assert(uf.find(2) != uf.find(3));

    std::cout << "✓ Find consistency test passed\n";
}

void test_validation_errors() {
    QuickFindUF uf(5);

    // Test invalid indices for find
    try {
        uf.find(-1);
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& e) {
        assert(std::string(e.what()) == "p not in range");
    }

    try {
        uf.find(5);
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& e) {
        assert(std::string(e.what()) == "p not in range");
    }

    // Test invalid indices for connected
    try {
        uf.connected(-1, 0);
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& e) {
        assert(std::string(e.what()) == "p not in range");
    }

    try {
        uf.connected(0, 5);
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& e) {
        assert(std::string(e.what()) == "p not in range");
    }

    // Test invalid indices for unite
    try {
        uf.unite(-1, 0);
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& e) {
        assert(std::string(e.what()) == "p not in range");
    }

    try {
        uf.unite(0, 5);
        assert(false);  // Should not reach here
    } catch (const std::invalid_argument& e) {
        assert(std::string(e.what()) == "p not in range");
    }

    std::cout << "✓ Validation errors test passed\n";
}

void test_complete_connection() {
    QuickFindUF uf(5);

    // Connect all elements into one component
    uf.unite(0, 1);
    uf.unite(1, 2);
    uf.unite(2, 3);
    uf.unite(3, 4);

    assert(uf.count() == 1);

    // Every pair should be connected
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            assert(uf.connected(i, j));
        }
    }

    std::cout << "✓ Complete connection test passed\n";
}

void test_large_dataset() {
    const int N = 1000;
    QuickFindUF uf(N);

    assert(uf.count() == N);

    // Connect even numbers together
    for (int i = 0; i < N; i += 2) {
        if (i + 2 < N) {
            uf.unite(i, i + 2);
        }
    }

    // Connect odd numbers together
    for (int i = 1; i < N; i += 2) {
        if (i + 2 < N) {
            uf.unite(i, i + 2);
        }
    }

    // Should have 2 components (even and odd)
    assert(uf.count() == 2);

    // All even numbers should be connected
    for (int i = 0; i < N; i += 2) {
        for (int j = 0; j < N; j += 2) {
            assert(uf.connected(i, j));
        }
    }

    // All odd numbers should be connected
    for (int i = 1; i < N; i += 2) {
        for (int j = 1; j < N; j += 2) {
            assert(uf.connected(i, j));
        }
    }

    // Even and odd should not be connected
    assert(!uf.connected(0, 1));
    assert(!uf.connected(2, 3));

    std::cout << "✓ Large dataset test passed\n";
}

void test_destructor_memory_management() {
    // Test that destructor properly cleans up memory
    // This test mainly verifies no crashes occur
    {
        QuickFindUF uf(100);
        for (int i = 0; i < 50; ++i) {
            uf.unite(i, i + 50);
        }
        // Destructor should be called here
    }

    std::cout << "✓ Destructor memory management test passed\n";
}

int main() {
    std::cout << "Running QuickFindUF tests...\n\n";

    test_constructor();
    test_single_element();
    test_basic_union();
    test_multiple_unions();
    test_union_large_components();
    test_redundant_unions();
    test_self_union();
    test_find_consistency();
    test_validation_errors();
    test_complete_connection();
    test_large_dataset();
    test_destructor_memory_management();

    std::cout << "\n🎉 All tests passed!\n";
    return 0;
}